#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glm/glm.hpp>

#include "shader.hpp"
#include "texture.hpp"

class SpriteRenderer {
public:
  //@ Constructor and Destructor method @//
  SpriteRenderer(Shader& sprite);

  //@ Draws an object with a rendered sprite @//
  void draw_object(glm::vec2 pos, glm::vec2 size, Texture& texture);
  void draw_object(glm::vec2 pos, glm::vec2 size, glm::vec4 value, bool is_round);

private:
  Shader shader;
  unsigned int round_quad_vao, sharp_quad_vao;
};

#endif
