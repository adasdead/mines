#ifndef GAME_COUNTER_H
#define GAME_COUNTER_H

#include "graphics/opengl.h"

#include "util/matrix4x4.h"
#include "util/basic_types.h"

struct counter {
    int                 value;
    mat4               *models;

    struct {
        GLuint          VAO;
        GLuint          VBO;
    } render;
};

typedef struct counter *counter_t;

counter_t counter_create(void);
void counter_update_x(counter_t counter, float x);
void counter_render(const counter_t counter, mat4 projection);
void counter_free(counter_t counter);

#endif /* GAME_COUNTER_H */