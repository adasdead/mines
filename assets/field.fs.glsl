#version 330 core

uniform sampler2D u_texture;
uniform ivec2 u_mouse_pos;

out vec4 frag_color;

in vec2 tex_coord;
in vec2 cell_pos;

void main()
{
    vec4 tex = texture(u_texture, tex_coord);
    
    if (cell_pos == u_mouse_pos) {
        frag_color = mix(tex, vec4(1.0f), 0.5f);
        return;
    }

    frag_color = tex;
}