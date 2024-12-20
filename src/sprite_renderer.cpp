#include "../include/sprite_renderer.hpp"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <iostream>

SpriteRenderer::SpriteRenderer(Shader &sprite):
shader(sprite)
{
  //@ Create vertex buffer and element buffer @//
  unsigned int VBO_round, EBO_round, VBO_sharp, EBO_sharp;

  //@ Create vertex data @//
  glm::vec2 vertices_round[24];
  float vertices_sharp[] = {
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f
  };

  vertices_round[0] = glm::vec2(0.0375f, 136.25f / 140.0f); //@ Top left @//
  vertices_round[1] = glm::vec2(0.9625f, 136.25f / 140.0f); //@ Top right @//
  vertices_round[2] = glm::vec2(0.9625f, 3.75f / 140.0f); //@ Bottom right @//
  vertices_round[3] = glm::vec2(0.0375f, 3.75f / 140.0f); //@ Bottom left @//
  
  for (int i = 0; i < 5; i++)
  {
    double theta = i * 0.125 * M_PI;
    double sinval = sin(theta) * 3 / 112.0, cosval = cos(theta) * 3 / 80.0;

    vertices_round[i+4] = glm::vec2(0.0375f - cosval, 136.25f / 140.0f + sinval);
    vertices_round[i+9] = glm::vec2(0.9625f + cosval, 136.25f / 140.0f + sinval);
    vertices_round[i+14] = glm::vec2(0.9625f + cosval, 3.75f / 140.0f - sinval);
    vertices_round[i+19] = glm::vec2(0.0375f - cosval, 3.75f / 140.0f - sinval);
  }

  //@ Create index data @//
  unsigned int indices_round[78];
  unsigned int indices_sharp[] = {
    0, 1, 2,
    0, 2, 3
  };

  for (int i = 0; i < 6; i++)
    indices_round[i] = indices_sharp[i];

  for (int i = 0; i < 4; i++)
  {
    int index = i * 3;
    indices_round[6 + index] = 4 + i;
    indices_round[7 + index] = 5 + i;
    indices_round[8 + index] = 0;

    indices_round[18 + index] = 9 + i;
    indices_round[19 + index] = 10 + i;
    indices_round[20 + index] = 1;

    indices_round[30 + index] = 14 + i;
    indices_round[31 + index] = 15 + i;
    indices_round[32 + index] = 2;

    indices_round[42 + index] = 19 + i;
    indices_round[43 + index] = 20 + i;
    indices_round[44 + index] = 3;
  }

  indices_round[54] = 8;
  indices_round[55] = 13;
  indices_round[56] = 1;

  indices_round[57] = 8;
  indices_round[58] = 1;
  indices_round[59] = 0;

  indices_round[60] = 9;
  indices_round[61] = 14;
  indices_round[62] = 2;

  indices_round[63] = 9;
  indices_round[64] = 2;
  indices_round[65] = 1;

  indices_round[66] = 18;
  indices_round[67] = 23;
  indices_round[68] = 3;

  indices_round[69] = 18;
  indices_round[70] = 3;
  indices_round[71] = 2;

  indices_round[72] = 19;
  indices_round[73] = 4;
  indices_round[74] = 0;

  indices_round[75] = 19;
  indices_round[76] = 0;
  indices_round[77] = 3;

  //@ Generate vertex array and buffer objects @//
  glGenVertexArrays(1, &this->round_quad_vao);
  glGenVertexArrays(1, &this->sharp_quad_vao);
  glGenBuffers(1, &VBO_round);
  glGenBuffers(1, &EBO_round);
  glGenBuffers(1, &VBO_sharp);
  glGenBuffers(1, &EBO_sharp);

  //@ Bind vertex array and buffer objects @//
  glBindVertexArray(this->round_quad_vao);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_round);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_round);

  //@ Add buffer data to buffer objects @//
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_round), vertices_round, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_round), indices_round, GL_STATIC_DRAW);

  //@ Set and enable vertex array attribute pointer @//
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
  glBindVertexArray(this->sharp_quad_vao);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_sharp);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_sharp);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_sharp), vertices_sharp, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_sharp), indices_sharp, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  //@ Unbind vertex arrays and buffer objecs @//
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  //@ Delete unnecessary buffer objects @//
  glDeleteBuffers(1, &VBO_round);
  glDeleteBuffers(1, &EBO_round);
  glDeleteBuffers(1, &VBO_sharp);
  glDeleteBuffers(1, &EBO_sharp);
}

void SpriteRenderer::draw_object(glm::vec2 pos, glm::vec2 size, Texture& texture)
{
  //@ Use shader program @//
  glUseProgram(this->shader.get_id());

  //@ Create model matrix @//
  glm::mat4 model(1.0f);

  //@ Move model matrix into position @//
  model = glm::translate(model, glm::vec3(pos, 0.0f));

  //@ Scale model matrix to size @//
  model = glm::scale(model, glm::vec3(size, 1.0f));

  //@ Send model matrix and sprite color to GPU @//
  this->shader.uniform_mat4("model", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(pos.x - 1.0f, pos.y - 1.0f, 0.0f)), glm::vec3(size.x * 102.0f / 100.0f, size.y * 142.0f / 140.0f, 1.0f)));
  this->shader.uniform_vec4("color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

  //@ Activate texture @//
  glActiveTexture(GL_TEXTURE0);
  texture.bind();

  //@ Draw the object @//
  glBindVertexArray(this->round_quad_vao);

  glDrawElements(GL_TRIANGLES, 78, GL_UNSIGNED_INT, 0);

  this->shader.uniform_mat4("model", model);
  this->shader.uniform_vec4("color", glm::vec4(1.0f));

  glDrawElements(GL_TRIANGLES, 78, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
}

void SpriteRenderer::draw_object(glm::vec2 pos, glm::vec2 size, glm::vec4 color, bool is_round)
{
  //@ Use shader program @//
  glUseProgram(this->shader.get_id());

  //@ Create and transform model matrix @//
  glm::mat4 model(1.0f);
  model = glm::translate(model, glm::vec3(pos, 0.0f));
  model = glm::scale(model, glm::vec3(size, 1.0f));

  //@ Bind no texture @//
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);

  //@ Draw the object @//
  if (is_round)
  {
    this->shader.uniform_mat4("model", model);
    this->shader.uniform_vec4("color", color);

    glBindVertexArray(this->round_quad_vao);
    glDrawElements(GL_TRIANGLES, 78, GL_UNSIGNED_INT, 0);
  }
  else
  {
    this->shader.uniform_mat4("model", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y - 1.0f, 0.0f)), glm::vec3(size.x, size.y * 62.0f / 60.0f, 1.0f)));
    this->shader.uniform_vec4("color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    glBindVertexArray(this->sharp_quad_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    this->shader.uniform_mat4("model", model);
    this->shader.uniform_vec4("color", color);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }
  glBindVertexArray(0);
}
