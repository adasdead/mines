#ifndef MATH_MATRIX4X4_H
#define MATH_MATRIX4X4_H

#include <stdlib.h>

#include "util/basic_types.h"

#define MATRIX4X4_WIDTH     4
#define MATRIX4X4_SIZE      MATRIX4X4_WIDTH * MATRIX4X4_WIDTH

typedef float *mat4;

/***************************************************************************/

static inline void matrix4x4_set(mat4 matrix, uint i, uint j, float value)
{
    if (matrix != NULL) {
        matrix[MATRIX4X4_WIDTH * i + j] = value;
    }
}

static inline float matrix4x4_get(mat4 matrix, uint i, uint j)
{
    return matrix[MATRIX4X4_WIDTH * i + j];
}

static inline void matrix4x4_free(mat4 matrix)
{
    if (matrix != NULL) {
        free(matrix);
    }
}

/***************************************************************************/

mat4 matrix4x4_allocate(bool unit);
mat4 matrix4x4_mult(mat4 dest, const mat4 src);

/*                              projection                              */

mat4 matrix4x4_ortho(float left, float right, float bottom, float top,
                     float near, float far);

/*                              transform                               */

mat4 matrix4x4_scale(mat4 dest, float x, float y);
mat4 matrix4x4_translate(mat4 dest, float x, float y);

#endif /* MATH_MATRIX4X4_H */