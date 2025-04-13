#version 460 core
in vec2 tex_coords;

out vec4 frag_color;

uniform sampler2D image;
uniform vec4 unif_color;
uniform bool use_texture;

void main()
{
  vec4 tex = use_texture ? texture(image, tex_coords) : vec4(1.0);
  vec4 color = unif_color;
  if (tex != vec4(1.0) && unif_color.w != 1.0)
  {
    tex.xz *= 1.0;
    tex.y *= 2.5 - unif_color.w;
    color.w = 1.0;
  }
  frag_color = vec4(1.0) * color * tex;
}
