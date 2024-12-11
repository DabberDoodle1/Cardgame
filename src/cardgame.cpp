#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../include/game.hpp"

int main()
{
  Game game(1280, 720, "OpenGL Sandbox Simulation");

  while (game.isRunning()) { game.update(); }

  glfwTerminate();
  return 0;
}
