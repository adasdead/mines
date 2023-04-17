#ifndef GAME_COUNTER_H
#define GAME_COUNTER_H

#include "graphics/opengl.h"

#include "math/matrix4x4.h"

#include "util/basic_types.h"

struct counter {
    i32 value;
    mat4 models[3];

    struct {
        GLuint VAO, VBO;
    } render;
};

typedef struct counter *counter_t;

counter_t counter_create(void);
void counter_update_x(counter_t counter, u32 x);
void counter_render(const counter_t counter, mat4 projection);
void counter_free(counter_t counter);

#endif /* GAME_COUNTER_H */