/* 
 * Copyright (c) 2023 adasdead
 * This software is licensed under the MIT License. (see the LICENSE file)
 */

#ifndef GAME_BORDER_H
#define GAME_BORDER_H

#include "graphics/renderer.h"

#include "util/matrix4x4.h"

struct border {
    uint                width;
    uint                height;
    size_t              size;

    byte               *frame;

    struct renderer     renderer;
};

typedef struct border *border_t;

border_t border_create(void);
void border_update(border_t border, int width, int height);
void border_render(const border_t border, mat4 projection);
void border_free(border_t border);

#endif /* GAME_BORDER_H */