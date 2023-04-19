#include "matrix4x4.h"

#include <string.h>

#include "util/logger.h"

mat4 matrix4x4_allocate(void)
{
    mat4 mat = calloc(MATRIX4X4_SIZE, sizeof *mat);

    if (mat == NULL) {
        logger_fatal("Failed to allocate memory for matrix4x4");
    }

    return mat;
}

mat4 matrix4x4_identity(mat4 mat)
{
    size_t i;

    if (mat != NULL) {
        memset(mat, 0, MATRIX4X4_SIZE * sizeof *mat);

        for (i = 0; i < MATRIX4X4_WIDTH; ++i) {
            matrix4x4_set(mat, i, i, 1.0f);
        }
    }

    return mat;
}

mat4 matrix4x4_mult(mat4 dest, const mat4 src)
{
    size_t i, j, k;
    mat4 result = matrix4x4_allocate();
    float x, y, z;

    if (dest != NULL && src != NULL) {
        for (i = 0; i < MATRIX4X4_WIDTH; ++i) {
            for (j = 0; j < MATRIX4X4_WIDTH; ++j) {
                for (k = 0; k < MATRIX4X4_WIDTH; ++k) {
                    x = matrix4x4_get(dest, i, k);
                    y = matrix4x4_get(src, k, j);
                    z = matrix4x4_get(result, i, j);

                    matrix4x4_set(result, i, j, z + (x * y));
                }
            }
        }

        memcpy(dest, result, MATRIX4X4_SIZE * sizeof *dest);
    }

    matrix4x4_free(result);
    return dest;
}

void matrix4x4_free(mat4 mat)
{
    if (mat != NULL) {
        free(mat);
    }
}

void matrix4x4_set(mat4 matrix, uint i, uint j, float value)
{
    if (matrix != NULL) {
        matrix[MATRIX4X4_WIDTH * i + j] = value;
    }
}

float matrix4x4_get(mat4 matrix, uint i, uint j)
{
    return matrix[MATRIX4X4_WIDTH * i + j];
}

mat4 matrix4x4_ortho(float left, float right, float bottom, float top,
                     float near, float far)
{
    mat4 matrix = matrix4x4_allocate();

    matrix4x4_identity(matrix);

    matrix4x4_set(matrix, 0, 0, 2.0f / (right - left));
    matrix4x4_set(matrix, 1, 1, 2.0f / (top - bottom));
    matrix4x4_set(matrix, 2, 2, 2.0f / (far - near));

    matrix4x4_set(matrix, 3, 0, -((right + left) / (right - left)));
    matrix4x4_set(matrix, 3, 1, -((top + bottom) / (top - bottom)));
    matrix4x4_set(matrix, 3, 2, -((far + near) / (far - near)));

    return matrix;
}

mat4 matrix4x4_scale(mat4 dest, float x, float y)
{
    mat4 matrix = matrix4x4_allocate();

    matrix4x4_identity(matrix);
    
    matrix4x4_set(matrix, 0, 0, x);
    matrix4x4_set(matrix, 1, 1, y);
    matrix4x4_mult(dest, matrix);
    matrix4x4_free(matrix);

    return dest;
}

mat4 matrix4x4_translate(mat4 dest, float x, float y)
{
    mat4 matrix = matrix4x4_allocate();

    matrix4x4_identity(matrix);

    matrix4x4_set(matrix, 3, 0, x);
    matrix4x4_set(matrix, 3, 1, y);
    matrix4x4_set(matrix, 3, 2, 1.0f);
    matrix4x4_mult(dest, matrix);
    matrix4x4_free(matrix);

    return dest;
}