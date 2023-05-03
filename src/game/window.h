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

void window_init(void);
void window_normalized_resize(int width, int height);
void window_free(void);

window_t window_instance(void);

#endif /* GAME_WINDOW_H */