#version 330 core

uniform mat4 u_projection;
uniform ivec2 u_field_pos;

in VS_OUT {
    uint tile;
} gs_in[];

out vec2 tex_coord;
out vec2 cell_pos;

const float TILE_SIZE = 16.0;

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

void main() {
    float offset = 1 / TILE_SIZE;
    float x = gs_in[0].tile / TILE_SIZE;
    vec4 pos = gl_in[0].gl_Position;

    cell_pos = vec2(pos.x, pos.y) - u_field_pos;

    gl_Position = u_projection * pos;
    tex_coord = vec2(x, TILE_SIZE);
    EmitVertex();

    gl_Position = u_projection * (pos + vec4(1.0, 0.0, 0.0, 0.0));
    tex_coord = vec2(x + offset, TILE_SIZE);
    EmitVertex();

    gl_Position = u_projection * (pos + vec4(0.0, 1.0, 0.0, 0.0));
    tex_coord = vec2(x, TILE_SIZE + offset);
    EmitVertex();

    gl_Position = u_projection * (pos + vec4(1.0, 1.0, 0.0, 0.0));
    tex_coord = vec2(x + offset, TILE_SIZE + offset);
    EmitVertex();

    EndPrimitive();
}