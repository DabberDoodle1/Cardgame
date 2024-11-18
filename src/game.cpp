#include "../include/game.hpp"
#include "../include/resource_manager.hpp"
#include <GLFW/glfw3.h>

Game::Game(unsigned int width, unsigned int height): state(GAME_ACTIVE), width(width), height(height), renderer(nullptr)
{
  //@ Create orthographic projection matrix @//
  glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);

  //@ Make and store shader @//
  ResourceManager::make_shader("res/shaders/sprite_vertex.glsl", "res/shaders/sprite_fragment.glsl", "sprite");

  //@ Send uniform data to the GPU @//
  Shader shader = ResourceManager::get_shader("sprite");
  glUseProgram(shader.get_id());
  shader.uniform_int("image", 0);
  shader.uniform_mat4("projection", projection);

  //@ Create sprite renderer @//
  renderer = new SpriteRenderer(shader);

  //@ Make texture @//
  //ResourceManager::make_texture("res/images/ace_spades.png", true, "face");
}

Game::~Game()
{
  delete renderer;
}

void Game::load_textures(std::string location, std::string extension, std::string cardnames[])
{
  for (int i = 0; i < 13; i++)
  {
    std::string str = cardnames[i];
    std::string s("_spades"), c("_clubs"), h("_hearts"), d("_diamonds");
    ResourceManager::make_texture(std::string(location + str + s + extension).c_str(), true, str + s);
    ResourceManager::make_texture(std::string(location + str + c + extension).c_str(), true, str + c);
    ResourceManager::make_texture(std::string(location + str + h + extension).c_str(), true, str + h);
    ResourceManager::make_texture(std::string(location + str + d + extension).c_str(), true, str + d);
  }
}

void Game::update(float delta_time)
{

}

void Game::render()
{
  renderer->draw_object(ResourceManager::get_texture("king_diamonds"), glm::vec2(0.0f), glm::vec2(500.0f, 700.0f));
}
