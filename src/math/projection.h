#ifndef MATH_PROJECTION_H
#define MATH_PROJECTION_H

#include "math/matrix4x4.h"

static mat4 projection_ortho(f32 left, f32 right, f32 bottom, f32 top,
                             f32 near, f32 far)
{
    mat4 matrix = matrix4x4_allocate(true);

    matrix4x4_set(matrix, 0, 0, 2.0f / (right - left));
    matrix4x4_set(matrix, 1, 1, 2.0f / (top - bottom));
    matrix4x4_set(matrix, 2, 2, 2.0f / (far - near));

    matrix4x4_set(matrix, 3, 0, -((right + left) / (right - left)));
    matrix4x4_set(matrix, 3, 1, -((top + bottom) / (top - bottom)));
    matrix4x4_set(matrix, 3, 2, -((far + near) / (far - near)));

    return matrix;
}

#endif /* MATH_PROJECTION_H */