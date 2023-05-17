#version 330 core

uniform mat4 u_projection;

in VS_OUT {
    uint tile;
} gs_in[];

out vec2 tex_coord;

const float ATLAS_SIZE = 256.0;
const float TILE_SIZE = 16.0;

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

void main() {
    float offset = TILE_SIZE / ATLAS_SIZE;
    float x = (gs_in[0].tile % 32) * offset;
    float y = (gs_in[0].tile / 32) * offset;
    y += 65 / ATLAS_SIZE;

    vec4 pos = gl_in[0].gl_Position;
    
    gl_Position = u_projection * pos;
    tex_coord = vec2(x, y);
    EmitVertex();
    
    gl_Position = u_projection * (pos + vec4(1.0, 0.0, 0.0, 0.0));
    tex_coord = vec2(x + offset, y);
    EmitVertex();
    
    gl_Position = u_projection * (pos + vec4(0.0, 1.0, 0.0, 0.0));
    tex_coord = vec2(x, y + offset);
    EmitVertex();
    
    gl_Position = u_projection * (pos + vec4(1.0, 1.0, 0.0, 0.0));
    tex_coord = vec2(x + offset, y + offset);
    
    EmitVertex();
    EndPrimitive();
}
