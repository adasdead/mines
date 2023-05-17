/* 
 * Copyright (c) 2023 adasdead
 * This software is licensed under the MIT License.
 */

#include "game/counter.h"

#include <string.h>
#include <math.h>

#include "util/resources.h"
#include "util/logger.h"

#define MAX(a, b) ((a > b) ? (a) : (b))
#define MIN(a, b) ((a < b) ? (a) : (b))

/*
 * Binary pow:
 * https://cp-algorithms.com/algebra/binary-exp.htm
 */
static long long b_pow(long long a, long long b)
{
    long long res = 1;

    while (b > 0) {
        if (b & 1)
            res = res * a;
        
        a *= a;
        b >>= 1;
    }

    return res;
}

static inline int counter_value(const counter_t counter)
{
    return (int) MAX(MIN(counter->value_max, counter->value), 0);
}

counter_t counter_create(void)
{
    size_t i;
    counter_t counter = malloc(sizeof *counter);

    if (counter == NULL)
        logger_fatal("Failed to allocate memory for counter");

    counter->models = calloc(COUNTER_NUMBERS, sizeof *counter->models);

    for (i = 0; i < COUNTER_NUMBERS; i++) {
        counter->models[i] = matrix4x4_allocate();
    }

    counter->value_max = b_pow(10, COUNTER_NUMBERS) - 1;

    /*
     * fill the array of powers of ten for easy digit extraction in
     * the future
     */
    for (i = 0; i < COUNTER_NUMBERS; i++) {
        counter->digit_base[i] = b_pow(10, (COUNTER_NUMBERS - 1) - i);
    }

    renderer_basic_initialize(&counter->renderer);
    return counter;
}

void counter_update_model_matrices(counter_t counter)
{
    size_t i;
    mat4 *cur = counter->models;
    
    static const float y = COUNTER_LY + COUNTER_OFFSET_LY;

    for (i = 0; i < COUNTER_NUMBERS; ++i, ++cur) {
        matrix4x4_identity(*cur);
        matrix4x4_scale(*cur, COUNTER_WIDTH, COUNTER_HEIGHT);
        /* shift each digit by COUNTER_WIDTH horizontally to draw */
        matrix4x4_translate(*cur, counter->x + i * COUNTER_WIDTH, y);
    }
}

void counter_render(const counter_t counter, mat4 projection)
{
    shader_t shader = resources_shader(RS_SHADER_COUNTER);
    const int n = counter_value(counter);
    int digit, i;

    shader_use(shader);
    shader_set_uniform_m4fv(shader, "u_projection", projection);

    texture_bind(resources_texture_atlas());

    glBindVertexArray(counter->renderer.VAO);

    for (i = 0; i < COUNTER_NUMBERS; ++i) {
        digit = n / counter->digit_base[i] % 10; /* extract digit */

        shader_set_uniform_m4fv(shader, "u_model", counter->models[i]);
        shader_set_uniform_1i(shader, "u_number", digit);
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);
    }

    glBindVertexArray(0);
}

void counter_free(counter_t counter)
{
    size_t i;

    if (counter != NULL) {
        for (i = 0; i < COUNTER_NUMBERS; ++i) {
            matrix4x4_free(counter->models[i]);
        }

        renderer_free(&counter->renderer);
        free(counter->models);
        free(counter);
    }
}