#version 460 core
in vec2 tex_coords;

out vec4 frag_color;

uniform sampler2D image;
uniform vec4 color;

void main()
{
  vec4 tex = color != vec4(1.0) ? vec4(1.0) : texture(image, tex_coords);
  frag_color = vec4(1.0) * color * tex;
}
