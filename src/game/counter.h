/* 
 * Copyright (c) 2023 adasdead
 * This software is licensed under the MIT License. (see the LICENSE file)
 */

#ifndef GAME_COUNTER_H
#define GAME_COUNTER_H

#include "graphics/renderer.h"

#include "util/matrix4x4.h"
#include "util/basic_types.h"

#include "definitions.h"

struct internal;

struct counter {
    int                 value;
    float               x;

    mat4               *models;
    struct renderer     renderer;

    long long           value_max;
    long long           digit_base[COUNTER_NUMBERS];
};

typedef struct counter *counter_t;

counter_t counter_create(void);
void counter_update_model_matrices(counter_t counter);
void counter_render(const counter_t counter, mat4 projection);
void counter_free(counter_t counter);

#endif /* GAME_COUNTER_H */