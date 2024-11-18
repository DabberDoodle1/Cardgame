#include "../include/sprite_renderer.hpp"

SpriteRenderer::SpriteRenderer(Shader& shader):
  shader(shader)
{
  
  //@ Creaete vertex buffer and element buffer @//
  unsigned int VBO, EBO;

  //@ Create vertex data @//
  glm::vec2 vertices[] = {
    glm::vec2(0.0f, 1.0f), //@ Top left @//
    glm::vec2(1.0f, 1.0f), //@ Top right @//
    glm::vec2(1.0f, 0.0f), //@ Bottom right @//
    glm::vec2(0.0f, 0.0f)  //@ Bottom left @//
  };

  //@ Create index data @//
  unsigned int indices[] = {
    0, 1, 2,
    0, 2, 3
  };

  //@ Generate vertex array and buffer objects @//
  glGenVertexArrays(1, &this->quad_vao);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  //@ Bind vertex array and buffer objects @//
  glBindVertexArray(quad_vao);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  //@ Add buffer data to buffer objects @//
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  //@ Set and enable vertex array attribute pointer @//
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  //@ Unbind vertex arrays and buffer objecs @//
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  //@ Delete unnecessary buffer objects @//
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

void SpriteRenderer::draw_object(Texture& texture, glm::vec2 pos, glm::vec2 size)
{
  //@ Use shader program @//
  glUseProgram(this->shader.get_id());

  //@ Create model matrix @//
  glm::mat4 model = glm::mat4(1.0f);

  //@ Move model matrix into position @//
  model = glm::translate(model, glm::vec3(pos, 0.0f));

  //@ Scale model matrix to size @//
  model = glm::scale(model, glm::vec3(size, 1.0f));

  //@ Send model matrix and sprite color to GPU @//
  this->shader.uniform_mat4("model", model);

  //@ Activate texture @//
  glActiveTexture(GL_TEXTURE0);
  texture.bind();

  //@ Draw the object @//
  glBindVertexArray(this->quad_vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
