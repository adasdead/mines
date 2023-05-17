/* 
 * Copyright (c) 2023 adasdead
 * This software is licensed under the MIT License.
 */

#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include "graphics/opengl.h"

#include "util/matrix4x4.h"
#include "util/basic_types.h"

struct window {
    bool                initialized;
    GLFWwindow         *handle;
    uint                width, height;
    mat4                projection;
    double              scale;

    struct {
        int             x, y;
    } cursor;
};

typedef struct window *window_t;

void window_initialize(void);
void window_resize_with_normalized_sizes(int width, int height);
void window_normalize_pos(int *x, int *y);
void window_free(void);

const window_t window_get_instance(void);

#endif /* GAME_WINDOW_H */