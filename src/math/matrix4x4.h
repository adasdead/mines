#ifndef MATH_MATRIX4X4_H
#define MATH_MATRIX4X4_H

#include <stdlib.h>

#include "util/basic_types.h"

#define MATRIX4X4_WIDTH     4
#define MATRIX4X4_SIZE      MATRIX4X4_WIDTH * MATRIX4X4_WIDTH

typedef f32 *mat4;

static inline void matrix4x4_set(mat4 matrix, u8 i, u8 j, f32 value)
{
    matrix[MATRIX4X4_WIDTH * i + j] = value;
}

static inline f32 matrix4x4_get(mat4 matrix, u8 i, u8 j)
{
    return matrix[MATRIX4X4_WIDTH * i + j];
}

static mat4 matrix4x4_allocate(bool unit)
{
    usize i;
    mat4 tmp = calloc(MATRIX4X4_SIZE, sizeof *tmp);

    if (unit) {
        for (i = 0; i < MATRIX4X4_WIDTH; i++) {
            matrix4x4_set(tmp, i, i, 1.0f);
        }
    }

    return tmp;
}

static void matrix4x4_free(mat4 matrix)
{
    free(matrix);
}

#endif /* MATH_MATRIX4X4_H */