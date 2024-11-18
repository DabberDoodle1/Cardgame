#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "texture.hpp"

class SpriteRenderer {
public:
  //@ Constructor and Destructor method @//
  SpriteRenderer(Shader& shader);

  //@ Draws an object with a rendered sprite @//
  void draw_object(Texture& texture, glm::vec2 pos, glm::vec2 size = glm::vec2(10.0f, 14.0f));
private:
  Shader shader;
  unsigned int quad_vao;
  void init();
};

#endif
