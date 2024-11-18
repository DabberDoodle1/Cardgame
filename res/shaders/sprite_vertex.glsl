#version 460 core
layout (location = 0) in vec2 vertex_pos;

out vec2 tex_coords;

uniform mat4 model;
uniform mat4 projection;

void main()
{
  tex_coords = vertex_pos;
  gl_Position = projection * model * vec4(vertex_pos, 0.0, 1.0);
}
