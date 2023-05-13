#version 330 core

uniform ivec2 u_size;

layout (location = 0) in uint tile;

out VS_OUT {
    uint tile;
} vs_out;

void main()
{
    float x = float(gl_VertexID % u_size.x);
    float y = float(gl_VertexID / u_size.x);
    
    gl_Position = vec4(x, y, 0, 1);

    vs_out.tile = tile;
}