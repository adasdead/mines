#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include "graphics/opengl.h"

#include "math/projection.h"

#include "util/basic_types.h"

void window_init(void);
void window_resize(u32 width, u32 height);
void window_normalize_mouse_pos(i32 *x, i32 *y);
void window_free(void);

double window_factor(void);
GLFWwindow *window_glfw(void);
mat4 window_projection(void);

#endif /* GAME_WINDOW_H */