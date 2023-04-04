#version 330 core

uniform vec2 u_smile_pos;
uniform mat4 u_projection;

layout (location = 0) in vec2 vertex;

out vec2 tex_coord;

const float SCALE_FACTOR = 0.655;

mat4 scale()
{
    mat4 scale_matrix;
    scale_matrix[0][0] = 1.0 + SCALE_FACTOR;
    scale_matrix[1][1] = 1.0 + SCALE_FACTOR;
    scale_matrix[2][2] = 1.0;
    scale_matrix[3][3] = 1.0;
    return scale_matrix;
}

mat4 transform()
{
    mat4 transform_matrix;
    transform_matrix[0][0] = 1.0;
    transform_matrix[3][0] = u_smile_pos.x + 0.15;
    transform_matrix[1][1] = 1.0;
    transform_matrix[3][1] = u_smile_pos.y + 0.15;
    transform_matrix[2][2] = 1.0;
    transform_matrix[3][3] = 1.0;
    return transform_matrix * scale();
}

void main()
{
    gl_Position = u_projection * transform() * vec4(vertex, 0.0, 1.0);
    tex_coord = vertex;
}