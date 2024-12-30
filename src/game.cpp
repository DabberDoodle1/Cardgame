#include "../include/game.hpp"
#include "../include/resource_manager.hpp"
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include <algorithm>

using std::string;
using std::random_device;
using std::uniform_int_distribution;

Cursor::Cursor(): x(0.), y(0.), card_x(0.), card_y(0.), card_held(nullptr), has_released(true), pile_selected(-1){}

Game::Game(unsigned int width, unsigned int height, const char *title):
  width(width), height(height), renderer(nullptr), window(nullptr), game_won(false), game_variation(SOLITAIRE_TURN_3), change_variant_next(false)
{
  init(width, height, title);
  load_textures("res/images/", ".png");
  new_game();
}

Game::~Game()
{
  delete renderer;
  clear_piles();
}

int Game::isRunning() const
{
  return !glfwWindowShouldClose(window);
}

void Game::update()
{
  input();

  if (check_game_won())
    game_won = true;

  render();

  glfwSwapBuffers(window);
  glfwPollEvents();
}

void Game::render()
{
  glClear(GL_COLOR_BUFFER_BIT);

  // Background
  renderer->draw_object(glm::vec2(0.f), glm::vec2(static_cast<float>(width), static_cast<float>(height)),
                        ResourceManager::get_texture("bg"), false, false, glm::vec4(2.f / 255.f, 152.f / 255.f, 15.f / 255.f, 0.35f));

  // Header
  renderer->draw_object(glm::vec2(0.f), glm::vec2(static_cast<float>(width), 60.f), false, true, glm::vec4(214.f / 255.f, 28.f / 255.f, 28.f / 255.f, 1.f));

  // Variant toggle
  renderer->draw_object(glm::vec2(30.f, 10.f), glm::vec2(280.f, 40.f), false, true, glm::vec4(1.f));
  renderer->draw_object(glm::vec2(169.f, 10.f), glm::vec2(2.f, 40.f), false, false, glm::vec4(0.f, 0.f, 0.f, 1.f));
  renderer->draw_object(glm::vec2(35.f, 15.f), glm::vec2(270.f, 30.f), ResourceManager::get_texture("toggle"), false, false);

  // Restart button
  renderer->draw_object(glm::vec2(static_cast<float>(width) - 52.5f, 7.5f), glm::vec2(45.f), true, true, glm::vec4(1.f));
  renderer->draw_object(glm::vec2(static_cast<float>(width) - 48.75f, 11.25f), glm::vec2(37.5f), ResourceManager::get_texture("restart"), true, true);


  glm::vec2 card_size(100.f, 140.f);

  // Stock pile
  if (!stock_pile.is_empty())
    renderer->draw_object(glm::vec2(140.f, 100.f), card_size, ResourceManager::get_texture( stock_pile.get_last()->get_texture() ), true, true);
  else
    renderer->draw_object(glm::vec2(140.f, 100.f), card_size, true, false, glm::vec4(1.f, 1.f, 1.f, 0.35f));

  // Waste pile
  if (!waste_pile.is_empty())
  {
    unsigned char pile_count = waste_pile.get_count();
    Card *current = waste_pile.get_first();
    int n;

    if (game_variation == SOLITAIRE_TURN_1)
      n = 1;
    else
      n = (cursor.pile_selected == 1 && cursor.card_held != nullptr) ? 2 : 3;
    
    while (pile_count > n)
    {
      current = current->next;
      pile_count--;
    }

    int i = 0;

    while (i < n)
    {
      if (!current)
        break;

      renderer->draw_object(glm::vec2(290.f + 25.f * i, 100.f), card_size, ResourceManager::get_texture( current->get_texture() ), true, true);
      current = current->next;
      i++;
    }
  }

  // Foundation piles
  for (int i = 0; i < 4; i++)
  {
    CardPile &pile = foundation_piles[i];
    glm::vec2 pos(590.f + 150.f * i, 100.f);

    if (!pile.is_empty())
      renderer->draw_object(pos, card_size, ResourceManager::get_texture( pile.get_last()->get_texture() ), true, true);
    else
      renderer->draw_object(pos, card_size, true, false, glm::vec4(1.f, 1.f, 1.f, 0.35f));
  }

  // Tableau piles
  for (int i = 0; i < 7; i++)
  {
    CardPile &pile = tableau_piles[i];
    glm::vec2 pos(140.f + 150.f * i, 300.f);

    if (!pile.is_empty())
    {
      Card *current = pile.get_first();
      unsigned int offset = 0;

      while (current != NULL)
      {
        renderer->draw_object(pos + glm::vec2(0.f, 20.f * offset), card_size, ResourceManager::get_texture( current->get_texture() ), true, true);
        offset++;

        current = current->next;
      }
    }
    else
      renderer->draw_object(pos, card_size, true, false, glm::vec4(1.f, 1.f, 1.f, 0.35f));
  }

  // Cards being held
  Card *hand_current = cursor.card_held;
  unsigned int offset = 0;

  while (hand_current != NULL)
  {
    renderer->draw_object(glm::vec2(cursor.x - cursor.card_x, cursor.y - cursor.card_y + 20.f * offset),
                          card_size, ResourceManager::get_texture( hand_current->get_texture() ), true, true);
    offset++;
    hand_current = hand_current->next;
  }

  if (game_won)
    renderer->draw_object(glm::vec2((static_cast<float>(width) / 2) - 405.f, 60.f), glm::vec2(810.f, 600.f),
                          ResourceManager::get_texture("its_winnable"), false, true);
}

void Game::input()
{
  //  Hover
  // -1       = nothing interactable hovered
  //  0       = stock pile
  //  1       = waste pile
  //  2-5     = foundation piles
  //  6-12    = tableau piles
  //  13-14   = game_version toggle switch
  //  15      = restart button
  //  Index
  //  -1      = last card of pile
  //   0      = no card selected
  //   1+     = nth card selected
  int hover = -1, index = 0;
  double card_x = 0.0, card_y = 0.0;

  glfwGetCursorPos(window, &cursor.x, &cursor.y);

  check_hitboxes(hover, index, card_x, card_y);

  // Determine if left mouse button is pressed or released during the frame
  if (!game_won)
  {
    switch (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
    {
      case GLFW_PRESS:
        if (cursor.has_released)
          press_response(hover, index, card_x, card_y);
        break;
      case GLFW_RELEASE:
        release_response(hover);
        break;
    }
  }
}

void Game::init(unsigned int width, unsigned int height, const char *title)
{
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(width, height, title, NULL, NULL);

  glfwMakeContextCurrent(window);

  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  // Callbacks
  glfwSetKeyCallback(window, key_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  glViewport(0, 0, 1280, 720);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Prepare the shader and shader uniforms when program starts so we don't
  // have to worry about it later when rendering
  ResourceManager::make_shader("res/shaders/sprite_vertex.glsl", "res/shaders/sprite_fragment.glsl", "sprite");
  Shader sprite_shader = ResourceManager::get_shader("sprite");
  glm::mat4 projection = glm::ortho(0.f, static_cast<float>(width), static_cast<float>(height), 0.f, -1.f, 1.f);

  glUseProgram(sprite_shader.get_id());
  sprite_shader.uniform_int("image", 0);
  sprite_shader.uniform_mat4("projection", &(projection[0].x));

  renderer = new SpriteRenderer(sprite_shader);
}

void Game::load_textures(string location, string extension)
{
  string faces[13] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "ace", "king", "queen", "jack" };

  for (string face : faces)
  {
    string s("_spades"), c("_clubs"), h("_hearts"), d("_diamonds");

    //@ Combine the strings to get the file path for 4 different suits @//
    ResourceManager::make_texture((location + face + s + extension).c_str(), face + s);
    ResourceManager::make_texture((location + face + c + extension).c_str(), face + c);
    ResourceManager::make_texture((location + face + h + extension).c_str(), face + h);
    ResourceManager::make_texture((location + face + d + extension).c_str(), face + d);
  }

  //@ Load all the homemade meme background sprites @//
  for (int i = 0; i < 4; i++)
  {
    string name("_bg");
    name = static_cast<char>(i + 49) + name;
    ResourceManager::make_texture((location + name + extension).c_str(), name);
  }

  ResourceManager::make_texture("res/images/background.jpg", string("bg"));
  ResourceManager::make_texture("res/images/restart.png", string("restart"));
  ResourceManager::make_texture("res/images/toggle.png", string("toggle"));
  ResourceManager::make_texture("res/images/its_winnable.jpeg", string("its_winnable"));
}

void Game::new_game()
{
  game_won = false;

  if (change_variant_next)
  {
    if (game_variation == SOLITAIRE_TURN_3)
      game_variation = SOLITAIRE_TURN_1;
    else
      game_variation = SOLITAIRE_TURN_3;

    change_variant_next = false;
  }

  clear_piles();

  Card* deck[52];
  random_device rd;

  for (int i = 0; i < 13; i++)
  {
    string bg("_bg");

    uniform_int_distribution<int> dist(0, 3);

    // Generate values from 0-3
    int n = dist(rd);
    int x = (n + 1) % 4;
    int y = (n + 2) % 4;
    int z = (n + 3) % 4;

    // Static cast after adding 49 so the values become '1' to '4'
    deck[i]    = new Card(0, i + 1, string(static_cast<char>(n + 49) + bg));
    deck[i+13] = new Card(1, i + 1, string(static_cast<char>(x + 49) + bg));
    deck[i+26] = new Card(2, i + 1, string(static_cast<char>(y + 49) + bg));
    deck[i+39] = new Card(3, i + 1, string(static_cast<char>(z + 49) + bg));
  }

  std::shuffle(std::begin(deck), std::end(deck), rd);
  std::shuffle(std::begin(deck), std::end(deck), rd);
  std::shuffle(std::begin(deck), std::end(deck), rd);

  for (int i = 0; i < 52; i++)
    stock_pile.append(deck[i]);

  for (int i = 0; i < 7; i++)
  {
    // Double loop with second loop starting at i to produce the ascending order
    // of card quantities from left to right of the tableau piles
    for (int j = i; j < 7; j++)
    {
      tableau_piles[j].append(stock_pile.get_last());
      stock_pile.remove_end();
    }

    tableau_piles[i].get_last()->flip();
  }
}

void Game::clear_piles()
{
  stock_pile.delete_pile();
  waste_pile.delete_pile();
  for (CardPile &pile : foundation_piles)
    pile.delete_pile();
  for (CardPile &pile : tableau_piles)
    pile.delete_pile();

  if (cursor.card_held)
  {
    CardPile temp_pile;
    temp_pile.append(cursor.card_held);
    temp_pile.delete_pile();
    cursor.card_held = nullptr;
  }
}

void Game::check_hitboxes(int& hover, int& index, double& card_x, double& card_y)
{
  for (int i = 0; i < 16; i++)
  {
    // first flipped x/y is only relevant for tableau piles
    // last card x/y is the main hitbox pair(misleading name for the restart button iteration) and will be accurate anyway for non-tableau piles
    float last_card_x, last_card_y;

    // Calculating hitbox ranges
    switch (i)
    {
      case 0:
        last_card_x = 140.f;
        last_card_y = 100.f;

        break;
      case 1:
        last_card_x = 290.f;
        last_card_y = 100.f;

        if (game_variation == SOLITAIRE_TURN_3)
        {
          if (waste_pile.get_count() > 3)
            last_card_x += 50.f;
          else
            last_card_x += 25.f * ( waste_pile.get_count() - 1 );
        }

        break;
      case 2 ... 5:
        last_card_x = 590.f + ( (i-2) * 150.f );
        last_card_y = 100.f;

        break;
      case 6 ... 12:
        if (cursor.card_held)
        {
          last_card_x = 140.f + ( (i - 6) * 150.f);
          last_card_y = 300.f + 20.f * ( tableau_piles[i - 6].get_count() - 1);

          break;
        }

        if (tableau_piles[i - 6].is_hovered(140.0 + ( (i - 6) * 150.0), cursor.x, cursor.y, index))
        {
          hover  = i;

          int offset;

          if (index == -1)
            offset = tableau_piles[i - 6].get_count() - 1;
          else
            offset = index - 1;

          card_x = 140.0 + ( (i - 6) * 150.0 );
          card_y = 300.0 + 20.0 * offset;

          return;
        }

        break;

      case 13:
        last_card_x = 30.f;
        last_card_y = 10.f;
        break;

      case 14:
        last_card_x = 170.f;
        last_card_y = 10.f;
        break;

      case 15:
        last_card_x = static_cast<float>(width) - 52.5f;
        last_card_y = 7.5f;

        break;
    }

    // Checking for hitbox range
    switch (i)
    {
      case 0 ... 12:
        if ( (cursor.x >= last_card_x && cursor.x <= last_card_x + 100.f)  &&  (cursor.y >= last_card_y && cursor.y <= last_card_y + 140.f) )
        {
          hover = i;

          if (i > 0)
          {
            card_x = last_card_x;
            card_y = last_card_y;
          }
        }

        break;
      case 13:
      case 14:
        if ( (cursor.x >= last_card_x && cursor.x <= last_card_x + 140.f)  &&  (cursor.y >= last_card_y && cursor.y <= last_card_y + 40.f) )
          hover = i;

        break;
      case 15:
        if ( (cursor.x >= last_card_x && cursor.x <= last_card_x + 45.f)  &&  (cursor.y >= last_card_y && cursor.y <= last_card_y + 45.f) )
          hover = i;

        break;
    }

    // Exit function when hovering over something to prevent unnecessary looping
    if (hover != -1)
      return;
  }
}

void Game::press_response(int hover, int index, double card_x, double card_y)
{
  cursor.has_released = false;

  switch (hover)
  {
    case 0:
      if (!stock_pile.is_empty())
      {
        int cards_to_flip;

        if (game_variation == SOLITAIRE_TURN_3)
          cards_to_flip = 3;
        else
          cards_to_flip = 1;

        for (int j = 0; j < cards_to_flip; j++)
        {
          if (stock_pile.is_empty())
            break;

          Card *last = stock_pile.get_last();

          last->flip();
          waste_pile.append(last);
          stock_pile.remove_end();
        }
      }

      else
      {
        while (!waste_pile.is_empty())
        {
          stock_pile.append( waste_pile.get_last() );
          stock_pile.get_last()->flip();
          waste_pile.remove_end();
        }
      }
      break;

    case 1:
      if (!waste_pile.is_empty())
      {
        cursor.card_held       = waste_pile.get_last();
        cursor.pile_selected   = 1;
        waste_pile.remove_end();

        cursor.card_x = cursor.x - card_x;
        cursor.card_y = cursor.y - card_y;
      }
      break;

    case 2 ... 5:
      {
        CardPile &pile = foundation_piles[ hover - 2 ];

        if (!pile.is_empty())
        {
          cursor.card_held     = pile.get_last();
          cursor.pile_selected = hover;
          pile.remove_end();

          cursor.card_x = cursor.x - card_x;
          cursor.card_y = cursor.y - card_y;
        }
      }
      break;

    case 6 ... 12:
      {
        CardPile &pile = tableau_piles[ hover - 6 ];

        if (!pile.is_empty())
        {
          Card *take = pile.take_nth(index);

          cursor.card_held     = take;
          cursor.pile_selected = hover;

          cursor.card_x = cursor.x - card_x;
          cursor.card_y = cursor.y - card_y;
        }
      }
      break;

    case 13:
      if (game_variation == SOLITAIRE_TURN_3)
        change_variant_next = true;
      break;

    case 14:
      if (game_variation == SOLITAIRE_TURN_1)
        change_variant_next = true;
      break;

    case 15:
      new_game();
      break;
  }
}

void Game::release_response(int hover)
{
  cursor.has_released = true;

  if (!cursor.card_held)
    return;

  if (hover == -1)
  {
    return_to_pile();
    return;
  }

  switch (hover)
  {
    case -1:
    case  0:
    case  1:
    case 20:
      return_to_pile();
      break;
    case  2 ... 5:
      {
        CardPile      &pile       = foundation_piles[hover - 2],
                      &prev       = find_pile(cursor.pile_selected);
        unsigned char cursor_face = cursor.card_held->get_face(),
                      cursor_suit = cursor.card_held->get_suit();

        if (pile.is_empty())
        {
          if (cursor_face == 1)
          {
            pile.append(cursor.card_held);
            cursor.card_held = nullptr;

            if (!prev.is_empty() && !prev.get_last()->face_is_on_top())
              prev.get_last()->flip();
          }
        }
        else if ((cursor_suit == pile.get_last()->get_suit())  &&  (cursor_face == pile.get_last()->get_face() + 1))
        {
          pile.append(cursor.card_held);
          cursor.card_held = nullptr;

          if (!prev.is_empty() && !prev.get_last()->face_is_on_top())
            prev.get_last()->flip();
        }
        else
          return_to_pile();
      }
      break;
    case  6 ... 12:
      {
        CardPile      &pile       = tableau_piles[hover - 6],
                      &prev       = find_pile(cursor.pile_selected);
        unsigned char cursor_face = cursor.card_held->get_face(),
                      cursor_suit = cursor.card_held->get_suit();

        if (pile.is_empty())
        {
          if (cursor_face == 13)
          {
            pile.append(cursor.card_held);
            cursor.card_held = nullptr;

            if (!prev.is_empty() && !prev.get_last()->face_is_on_top())
              prev.get_last()->flip();
          }
          else
            return_to_pile();
        }

        else if ( (cursor_suit / 2 != pile.get_last()->get_suit() / 2)  && (cursor_face == pile.get_last()->get_face() - 1) )
        {
          pile.append(cursor.card_held);
          cursor.card_held = nullptr;

          if (!prev.is_empty() && !prev.get_last()->face_is_on_top())
            prev.get_last()->flip();
        }

        else
          return_to_pile();
      }
      break;
  }
}

void Game::return_to_pile()
{
  CardPile &pile = find_pile(cursor.pile_selected);
  pile.append(cursor.card_held);
  cursor.card_held = nullptr;
}

CardPile& Game::find_pile(int index)
{
  CardPile *pile;

  switch (index)
  {
    case 1:
      pile = &waste_pile;
      break;
    case 2 ... 5:
      pile = &foundation_piles[index - 2];
      break;
    case 6 ... 12:
      pile = &tableau_piles[index - 6];
      break;
  }

  return *pile;
}

bool Game::check_game_won()
{
  for (int i = 0; i < 4; i++)
  {
    int current_face = 1;
    Card *current_card = foundation_piles[i].get_first();

    while (current_face <= 13)
    {
      if (!current_card)
        return false;

      if (current_card->get_face() != current_face)
        return false;

      current_card = current_card->next;
      current_face++;
    }
  }

  return true;
}

void Game::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void Game::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}
