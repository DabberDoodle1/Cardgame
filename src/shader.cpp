#include "../include/shader.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Shader::Shader() { }

Shader::Shader(const char* vertex_source, const char* fragment_source)
{
  unsigned int vertex, fragment;
  int success;
  char error_log[1024];

  //@ Vertex shader @//
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vertex_source, NULL);
  glCompileShader(vertex);
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertex, 1024, NULL, error_log);
    std::cout << "VERTEX SHADER ERROR FOUND!\nReason: " << error_log << std::endl;
  }

  //@ Fragment shader @//
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fragment_source, NULL);
  glCompileShader(fragment);
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragment, 1024, NULL, error_log);
    std::cout << "FRAGMENT SHADER ERROR FOUND!\nReason: " << error_log << std::endl;
  }

  //@ Shader program @//
  this->ID = glCreateProgram();
  glAttachShader(this->ID, vertex);
  glAttachShader(this->ID, fragment);
  glLinkProgram(this->ID);
  glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(this->ID, 1024, NULL, error_log);
    std::cout << "SHADER PROGRAM ERROR FOUND!\nReason: " << error_log << std::endl;
  }

  //@ Deleting unnecessary shaders @//
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

unsigned int& Shader::get_id()
{
  return ID;
}

void Shader::uniform_mat4(const char* name, glm::mat4 value) const
{
  glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, false, glm::value_ptr(value));
}

void Shader::uniform_vec4(const char* name, glm::vec4 value) const
{
  glUniform4fv(glGetUniformLocation(this->ID, name), 1, glm::value_ptr(value));
}

void Shader::uniform_int(const char* name, int value) const
{
  glUniform1i(glGetUniformLocation(this->ID, name), value);
}

void Shader::uniform_float(const char* name, float value) const
{
  glUniform1f(glGetUniformLocation(this->ID, name), value);
}
