#version 330 core

in vec2 tex_coord;

uniform sampler2D u_texture;
uniform int u_number;

const float OFFSET_Y_PX = 16.0;
const float ATLAS_PX = 256.0;
const float COUNTER_Y_PX = 23.0;
const float COUNTER_X_PX = 13.0;

out vec4 frag_color;

void main()
{
    float offset_x = (COUNTER_X_PX * u_number) / ATLAS_PX;
    float uv_x = tex_coord.x / (ATLAS_PX / COUNTER_X_PX);
    float uv_y = tex_coord.y / (ATLAS_PX / COUNTER_Y_PX);
    vec2 uv = vec2(uv_x + offset_x, uv_y + OFFSET_Y_PX / ATLAS_PX);
    frag_color = texture(u_texture, uv);
}  