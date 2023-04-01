#version 330 core

uniform ivec2 u_field_size;
uniform ivec2 u_field_pos;

layout (location = 0) in uint tile;

out VS_OUT {
    uint tile;
    int cell_opened;
} vs_out;

void main()
{
    float x = float(gl_VertexID % u_field_size.x);
    float y = float(gl_VertexID / u_field_size.x);
    
    gl_Position = vec4(u_field_pos.x + x, u_field_pos.y + y, 0, 1);

    vs_out.cell_opened = 0;

    switch ((tile >> 4u)) {
        case 0u:                            /* CELL_STATE_CLOSED */
            vs_out.tile = 12u;
            break;
        case 1u:                            /* CELL_STATE_OPENED */
            vs_out.tile = tile & 15u;
            vs_out.cell_opened = 1;
            break;
        case 2u:                            /* CELL_STATE_FLAGGED */
            vs_out.tile = 14u;
            break;
        case 3u:                            /* CELL_STATE_QUESTIONED */
            vs_out.tile = 13u;
    }
}