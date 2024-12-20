#include "../include/game.hpp"
#include "../include/resource_manager.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>
#include <algorithm>

Game::Game(unsigned int width, unsigned int height, const char* title): state(GAME_ACTIVE), width(width), height(height), renderer(nullptr), window(nullptr), slots()
{
  //@ Initialize GLFW library @//
  if (!glfwInit())
  {
    std::cout << "ERROR: GLFW NOT INITIALIZED" << std::endl;
    glfwTerminate();
    return;
  }

  //@ GLFW window hints @//
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  //@ Create the window @//
  window = glfwCreateWindow(1280, 720, "OpenGL Sandbox Simulation", NULL, NULL);

  if (!window)
  {
    std::cout << "ERROR: WINDOW NOT CREATED" << std::endl;
    glfwTerminate();
    return;
  }

  glfwMakeContextCurrent(window);


  //@ Initialize GLAD library @//
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "ERROR: GLAD NOT INITIALIZED" << std::endl;
    glfwTerminate();
    return;
  }

  //@ Set GLFW callback functions @//
  glfwSetKeyCallback(window, this->key_callback);
  glfwSetFramebufferSizeCallback(window, this->framebuffer_size_callback);

  //@ Set OpenGL parameters @//
  glViewport(0, 0, 1280, 720);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  //@ Create orthographic projection matrix @//
  glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);

  //@ Make and store shader @//
  ResourceManager::make_shader("res/shaders/sprite_vertex.glsl", "res/shaders/sprite_fragment.glsl", "sprite");

  //@ Send uniform data to the GPU @//
  Shader sprite_shader = ResourceManager::get_shader("sprite");

  glUseProgram(sprite_shader.get_id());
  sprite_shader.uniform_int("image", 0);
  sprite_shader.uniform_mat4("projection", projection);

  //@ Create sprite renderer @//
  renderer = new SpriteRenderer(sprite_shader);

  //@ Load all texture sprites into Texture objects @//
  std::string faces[13] = {
    "2", "3", "4", "5", "6", "7", "8", "9", "10", "ace", "king", "queen", "jack"
  };

  this->load_textures("res/images/", ".png", faces);


  //@ Randomly load all the Card objects to be used@//
  Card* cards[52];
  std::random_device rd;

  for (int i = 0; i < 13; i++)
  {
    std::uniform_int_distribution<int> dist(1, 4);
    int n = dist(rd);
    std::string bg("_bg");

    int x = (n + 1) % 4;
    int y = (n + 2) % 4;
    int z = (n + 3) % 4;

    if (n == 0 || x == 0 || y == 0 || z == 0)
    {
      n++;
      x++;
      y++;
      z++;
    }

    cards[i] = new Card(i+1, 1, std::string(static_cast<char>(n + 48) + bg).c_str());
    cards[i + 13] = new Card(i+1, 2, std::string(static_cast<char>(x + 48) + bg).c_str());
    cards[i + 26] = new Card(i+1, 3, std::string(static_cast<char>(y + 48) + bg).c_str());
    cards[i + 39] = new Card(i+1, 4, std::string(static_cast<char>(z + 48) + bg).c_str());
  }

  std::shuffle(&cards[0], &cards[52], rd);

  for (int i = 0; i < 13; i++)
  {
    switch (i)
    {
      case 0:
        slots[0] = CardSlot(140.0f, 100.0f, 0);
        break;
      case 1:
        slots[1] = CardSlot(290.0f, 100.0f, -1);
        break;
      case 2 ... 5:
        slots[i] = CardSlot(140.0f + (i+1) * 150.0f, 100.0f, 0);
        break;
      case 6 ... 12:
        slots[i] = CardSlot(140.0f + (i-6) * 150.0f, 300.0f, 1);
        break;
    }
  }

  for (int i = 0; i < 52; i++)
  {
    slots[0].add_card(cards[i], false);
    cards[i] = nullptr;
  }

  for (int i = 0; i < 7; i++)
  {
    for (int k = i; k < 7; k++)
    {
      slots[6 + k].add_card(slots[0].get_last_card(), false);
      slots[0].remove_card();
    }
  }
}

Game::~Game()
{
  delete renderer;
  for (int i = 0; i < 13; i++)
    slots[i].clear();
}

void Game::load_textures(std::string location, std::string extension, std::string faces[])
{
  for (int i = 0; i < 13; i++)
  {
    std::string str = faces[i];
    std::string s("_spades"), c("_clubs"), h("_hearts"), d("_diamonds");
    ResourceManager::make_texture(std::string(location + str + s + extension).c_str(), str + s);
    ResourceManager::make_texture(std::string(location + str + c + extension).c_str(), str + c);
    ResourceManager::make_texture(std::string(location + str + h + extension).c_str(), str + h);
    ResourceManager::make_texture(std::string(location + str + d + extension).c_str(), str + d);
  }
  
  for (int i = 0; i < 4; i++)
  {
    char c = static_cast<char>(i + 49);
    std::string common("_bg");
    ResourceManager::make_texture(std::string(location + c + common + extension).c_str(), std::string(c + common));
  }
}

int Game::isRunning()
{
  return !glfwWindowShouldClose(window);
}

void Game::update()
{
  glfwPollEvents();

  glClearColor(2.0f / 255.0f, 152.0f / 255.0f, 15.0f / 255.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  this->render();

  glfwSwapBuffers(window);
}

void Game::render()
{
  renderer->draw_object(glm::vec2(0.0f), glm::vec2(1280.0f, 60.0f), glm::vec4(172.0f / 255.0f, 54.0f / 255.0f, 35.0f / 255.0f, 1.0f), false);

  glm::vec2 card_size(100.0f, 140.0f);
  for (int i = 0; i < 13; i++)
  {
    CardSlot slot = slots[i];
    float slot_x = slot.get_x(), slot_y = slot.get_y();

    if(slot.get_expansion() == -1)
    {
      Card *card = slot.get_first_card();

      if (card)
      {}
    }

    else if (slot.get_expansion() == 0)
    {
      Card *card = slot.get_last_card();

      if (card)
        renderer->draw_object(glm::vec2(slot_x, slot_y), card_size, ResourceManager::get_texture(card->get_current_texture()));
      else
        renderer->draw_object(glm::vec2(slot_x, slot_y), card_size, glm::vec4(1.0f, 1.0f, 1.0f, 0.15f), true);
    }

    else {
      Card *cards = slot.get_first_card();
      
      if (cards)
        for (int i = 0; cards != NULL; i++)
        {
          renderer->draw_object(glm::vec2(slot_x, slot_y + i * 9.625f), card_size, ResourceManager::get_texture(cards->get_current_texture()));
          cards = cards->next;
        }
      else
        renderer->draw_object(glm::vec2(slot_x, slot_y), card_size, glm::vec4(1.0f, 1.0f, 1.0f, 0.15f), true);
    }
  }
}

void Game::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void Game::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}
