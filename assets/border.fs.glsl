#version 330 core

uniform sampler2D u_texture;

out vec4 frag_color;

in vec2 tex_coord;

void main()
{
    frag_color = texture(u_texture, tex_coord);
}