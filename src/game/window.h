#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include "graphics/opengl.h"

#include "math/matrix4x4.h"

#include "util/basic_types.h"

void window_init(void);
void window_resize(uint width, uint height);
void window_normalize_cursor_pos(int *x, int *y);
void window_cursor_pos(int *x, int *y);
double window_scale_factor(void);
void window_free(void);

GLFWwindow *window_glfw(void);
mat4 window_projection(void);

#endif /* GAME_WINDOW_H */