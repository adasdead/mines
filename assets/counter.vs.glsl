#version 330 core

uniform mat4 u_projection;
uniform mat4 u_model;

layout (location = 0) in vec2 vertex;

out vec2 tex_coord;

void main()
{
    gl_Position = u_projection * u_model * vec4(vertex, 0.0, 1.0);
    tex_coord = vertex;
}