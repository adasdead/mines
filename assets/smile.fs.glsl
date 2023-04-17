#version 330 core

in vec2 tex_coord;

uniform sampler2D u_texture;
uniform int u_smile_state;

const float OFFSET_Y_PX = 39.0;
const float ATLAS_PX = 256.0;
const float SMILE_PX = 26.0;

out vec4 frag_color;

void main()
{
    float offset_x = (SMILE_PX * u_smile_state) / ATLAS_PX;
    vec2 uv = tex_coord / (ATLAS_PX / SMILE_PX);
    uv += vec2(offset_x, OFFSET_Y_PX / ATLAS_PX);
    frag_color = texture(u_texture, uv);
}