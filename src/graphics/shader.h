#ifndef GRAPHICS_SHADER_H
#define GRAPHICS_SHADER_H

#include "glad/glad.h"

#include "math/matrix4x4.h"

typedef GLuint shader_t;

shader_t shader_load(const char *vertex_path, const char *fragment_path);

void shader_use(shader_t shader);
void shader_set_uniform_1i(shader_t shader, const char *name, int value);
void shader_set_uniform_m4fv(shader_t shader, const char *name, m4x4 matrix);
void shader_free(shader_t shader);

#endif /* GRAPHICS_SHADER_H */