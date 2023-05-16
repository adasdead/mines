/* 
 * Copyright (c) 2023 adasdead
 * This software is licensed under the MIT License. (see the LICENSE file)
 */

#ifndef GRAPHICS_SHADER_H
#define GRAPHICS_SHADER_H

#include "graphics/opengl.h"

#include "util/matrix4x4.h"

typedef GLuint shader_t;

shader_t shader_load(const char *vertex_path, const char *fragment_path);
shader_t shader_load_g(const char *vertex_path, const char *fragment_path,
                       const char *geometry_path);

void shader_use(shader_t shader);
void shader_set_uniform_1i(shader_t shader, const char *name, int value);
void shader_set_uniform_m4fv(shader_t shader, const char *name, mat4 matrix);
void shader_set_uniform_2i(shader_t shader, const char *name, int x, int y);
void shader_set_uniform_2f(shader_t shader, const char *name, float x, float y);
void shader_free(shader_t shader);

#endif /* GRAPHICS_SHADER_H */