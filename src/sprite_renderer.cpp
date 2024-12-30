#include "../include/sprite_renderer.hpp"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <glm/gtc/type_ptr.hpp>

SpriteRenderer::SpriteRenderer(Shader &sprite): shader(sprite)
{
  unsigned int VBO_round, EBO_round, VBO_sharp, EBO_sharp;

  glm::vec2 vertices_round[24];
  float vertices_sharp[] = {
    0.f, 1.f,
    1.f, 1.f,
    1.f, 0.f,
    0.f, 0.f
  };

  vertices_round[0] = glm::vec2(0.05f, 27.f / 28.f); // Top left
  vertices_round[1] = glm::vec2(0.95f, 27.f / 28.f); // Top right
  vertices_round[2] = glm::vec2(0.95f, 1.f / 28.f);   // Bottom right
  vertices_round[3] = glm::vec2(0.05f, 1.f / 28.f);   // Bottom left

  // Quarter circle math for the vertices of each corner
  for (int i = 0; i < 5; i++)
  {
    double theta = i * 0.125 * M_PI;
    double sinval = sin(theta) / 28.0, cosval = cos(theta) * 0.05;

    vertices_round[i+4] = glm::vec2(0.05f - cosval, 27.f / 28.f + sinval);
    vertices_round[i+9] = glm::vec2(0.95f + cosval, 27.f / 28.f + sinval);
    vertices_round[i+14] = glm::vec2(0.95f + cosval, 1.f / 28.f - sinval);
    vertices_round[i+19] = glm::vec2(0.05f - cosval, 1.f / 28.f - sinval);
  }

  // Index array to control the edge and face formation of the vertices while minimizing bloat
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

  // Generate vertex array and buffer objects
  glGenVertexArrays(1, &round_quad_vao);
  glGenVertexArrays(1, &sharp_quad_vao);
  glGenBuffers(1, &VBO_round);
  glGenBuffers(1, &EBO_round);
  glGenBuffers(1, &VBO_sharp);
  glGenBuffers(1, &EBO_sharp);

  // Time to set up round quad vertex array
  glBindVertexArray(round_quad_vao);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_round);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_round), vertices_round, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_round);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_round), indices_round, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
  glEnableVertexAttribArray(0);


  // Time to set up sharp quad vertex array
  glBindVertexArray(0);
  glBindVertexArray(sharp_quad_vao);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_sharp);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_sharp), vertices_sharp, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_sharp);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_sharp), indices_sharp, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Delete the VBOs and EBOs since they are unnecessary now
  glDeleteBuffers(1, &VBO_round);
  glDeleteBuffers(1, &EBO_round);
  glDeleteBuffers(1, &VBO_sharp);
  glDeleteBuffers(1, &EBO_sharp);
}

void SpriteRenderer::draw_object(glm::vec2 pos, glm::vec2 size, Texture& texture, bool is_round, bool has_border, glm::vec4 color)
{
  unsigned int indices;
  glUseProgram(shader.get_id());

  glm::mat4 model(1.f), border(1.f);

  model = glm::translate(model, glm::vec3(pos, 0.f));
  model = glm::scale(model, glm::vec3(size, 1.f));

  border = glm::translate(border, glm::vec3(pos.x - 1.f, pos.y - 1.f, 0.f));
  border = glm::scale(border, glm::vec3(size.x + 2.f, size.y + 2.f, 1.f));

  // Bind the texture to use
  glActiveTexture(GL_TEXTURE0);
  texture.bind();
  shader.uniform_int("use_texture", 1);

  // Binding the vertex array needed
  if (is_round)
  {
    glBindVertexArray(round_quad_vao);
    indices = 78;
  } else {
    glBindVertexArray(sharp_quad_vao);
    indices = 6;
  }

  if (has_border)
  {
    glm::vec4 border_color(0.f, 0.f, 0.f, 1.f);
    shader.uniform_mat4("model", &(border[0].x));
    shader.uniform_vec4("unif_color", &(border_color.x));
    glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0);
  }

  // Draw the actual card
  shader.uniform_mat4("model", &(model[0].x));
  shader.uniform_vec4("unif_color", &(color.x));
  glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
}

void SpriteRenderer::draw_object(glm::vec2 pos, glm::vec2 size, bool is_round, bool has_border, glm::vec4 color)
{
  unsigned int indices;
  glUseProgram(shader.get_id());

  glm::mat4 model(1.f), border(1.f);

  model = glm::translate(model, glm::vec3(pos, 0.f));
  model = glm::scale(model, glm::vec3(size, 1.f));

  border = glm::translate(border, glm::vec3(pos.x - 1.f, pos.y - 1.f, 0.f));
  border = glm::scale(border, glm::vec3(size.x + 2.f, size.y + 2.f, 1.f));

  // Unbinding texture at texture unit 0
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);
  shader.uniform_int("use_texture", 0);

  if (is_round)
  {
    glBindVertexArray(round_quad_vao);
    indices = 78;
  } else {
    glBindVertexArray(sharp_quad_vao);
    indices = 6;
  }

  if (has_border)
  {
    glm::vec4 border_color(0.f, 0.f, 0.f, 1.f);
    shader.uniform_mat4("model", &(border[0].x));
    shader.uniform_vec4("unif_color", &(border_color.x));
    glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0);
  }

  // Draw the actual card
  shader.uniform_mat4("model", &(model[0].x));
  shader.uniform_vec4("unif_color", &(color.x));
  glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
}
