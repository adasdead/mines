/* 
 * Copyright (c) 2023 adasdead
 * This software is licensed under the MIT License.
 */

#ifndef MATH_MATRIX4X4_H
#define MATH_MATRIX4X4_H

#include <stdlib.h>

#include "util/basic_types.h"

#define MATRIX4X4_WIDTH                 4
/* MATRIX4X4_WIDTH * MATRIX4X4_WIDTH */
#define MATRIX4X4_SIZE                  16

typedef float *mat4;

mat4 matrix4x4_allocate(void);
mat4 matrix4x4_identity(mat4 mat);
mat4 matrix4x4_mult(mat4 restrict dest, const mat4 src);
void matrix4x4_free(mat4 mat);

void matrix4x4_set(mat4 matrix, uint i, uint j, float value);
float matrix4x4_get(const mat4 matrix, uint i, uint j);

mat4 matrix4x4_ortho(float left, float right, float bottom, float top,
                     float near, float far);

mat4 matrix4x4_scale(mat4 restrict dest, float x, float y);
mat4 matrix4x4_translate(mat4 restrict dest, float x, float y);

#endif /* MATH_MATRIX4X4_H */