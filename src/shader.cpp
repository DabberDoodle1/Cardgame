#include "../include/shader.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader() { }

Shader::Shader(const char *vertex_source, const char *fragment_source)
{
  unsigned int vertex, fragment;

  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vertex_source, NULL);
  glCompileShader(vertex);

  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fragment_source, NULL);
  glCompileShader(fragment);

  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

unsigned int& Shader::get_id()
{
  return ID;
}

void Shader::uniform_mat4(const char *name, const float *value) const
{
  glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, false, value);
}

void Shader::uniform_vec4(const char *name, const float *value) const
{
  glUniform4fv(glGetUniformLocation(ID, name), 1, value);
}

void Shader::uniform_int(const char *name, int value) const
{
  glUniform1i(glGetUniformLocation(ID, name), value);
}

void Shader::uniform_float(const char *name, float value) const
{
  glUniform1f(glGetUniformLocation(ID, name), value);
}
