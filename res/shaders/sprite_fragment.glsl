#version 460 core
in vec2 tex_coords;

out vec4 frag_color;

uniform sampler2D image;

void main()
{
  frag_color = vec4(1.0) * texture(image, tex_coords);
}
