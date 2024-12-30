#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glm/glm.hpp>
#include "shader.hpp"
#include "texture.hpp"

class SpriteRenderer {
public:
  SpriteRenderer(Shader& sprite);

  void          draw_object(glm::vec2 pos, glm::vec2 size, Texture& texture, bool is_round, bool has_border, glm::vec4 color = glm::vec4(1.0f));
  void          draw_object(glm::vec2 pos, glm::vec2 size, bool is_round, bool has_border, glm::vec4 color);
private:
  Shader        shader;
  unsigned int  round_quad_vao,
                sharp_quad_vao;
};

#endif
