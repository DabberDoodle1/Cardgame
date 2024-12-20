#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include "sprite_renderer.hpp"
#include "cards.hpp"

enum GameState {
  GAME_ACTIVE,
  GAME_WIN,
  GAME_PAUSE
};

class Game {
public:
  //@ Constructor and destructor @//
  Game(unsigned int width, unsigned int height, const char* title);
  ~Game();

  //@ Load all textures into ResourceManager @//
  void load_textures(std::string location, std::string extension, std::string faces[]);

  //@ Basic public methods to run game @//
  int isRunning();
  void update();

private:
  //@ Essential variables @//
  SpriteRenderer* renderer;
  GameState state;
  bool keys[1024];
  unsigned int width, height;
  GLFWwindow* window;
  CardSlot slots[13];

  //@ Private static methods @//
  static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

  //@ Basic private methods to run game @//
  void render();
  void input();
};

#endif
