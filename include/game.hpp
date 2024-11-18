#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include "sprite_renderer.hpp"

enum GameState {
  GAME_ACTIVE,
  GAME_WIN,
  GAME_PAUSE
};

class Game {
public:
  //@ Constructor and destructor @//
  Game(unsigned int width, unsigned int height);
  ~Game();

  //@ Load all textures into ResourceManager @//
  void load_textures(std::string location, std::string extension, std::string cardnames[]);

  //@ Basic necessary methods to run game @//
  void update(float delta_time);
  void render();

private:
  //@ Essential variables @//
  SpriteRenderer* renderer;
  GameState state;
  bool keys[1024];
  unsigned int width, height;
};

#endif
