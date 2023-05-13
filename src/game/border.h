#ifndef GAME_BORDER_H
#define GAME_BORDER_H

#include "graphics/opengl.h"

#include "util/matrix4x4.h"

struct border {
    uint                width;
    uint                height;

    byte               *frame;

    OPENGL_RENDER_STRUCT;
};

typedef struct border *border_t;

border_t border_create(void);
void border_update_frame(border_t border, int width, int height);
void border_render(const border_t border, mat4 projection);
void border_free(border_t border);

#endif /* GAME_BORDER_H */