#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "../include/game.hpp"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
  if (!glfwInit())
  {
    std::cout << "ERROR: GLFW NOT INITIALIZED" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  GLFWwindow* window = glfwCreateWindow(1280, 720, "OpenGL Sandbox Simulation", NULL, NULL);

  if (!window)
  {
    std::cout << "ERROR: WINDOW NOT CREATED" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "ERROR: GLAD NOT INITIALIZED" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwSetKeyCallback(window, key_callback);
  glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

  glViewport(0, 0, 1280, 720);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Game game(1280, 720);

  std::string files[13] = {
    "2", "3", "4", "5", "6", "7", "8", "9", "10", "ace", "king", "queen", "jack"
  };

  game.load_textures("res/images/", ".png", files);

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glClearColor(0.5f, 0.2f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    game.render();

    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}
