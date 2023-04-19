#include "game/counter.h"

#include <string.h>

#include "util/resources.h"
#include "util/logger.h"

#include "definitions.h"

#define MAX(a, b) ((a > b) ? (a) : (b))
#define MIN(a, b) ((a < b) ? (a) : (b))

static long long binary_pow(long long n, long long x)
{
    if (x == 0) return 1;
    if (x % 2 == 0) return binary_pow(n * n, x / 2);
    return n * binary_pow(n * n, x / 2);
}

counter_t counter_create(void)
{
    size_t i;
    counter_t counter = malloc(sizeof *counter);

    if (counter != NULL) {
        counter->models = calloc(COUNTER_NUMBERS, sizeof *counter->models);

        for (i = 0; i < COUNTER_NUMBERS; i++) {
            counter->models[i] = matrix4x4_allocate();
        }

        glGenVertexArrays(1, &counter->render.VAO);
        glGenBuffers(1, &counter->render.VBO);

        glBindBuffer(GL_ARRAY_BUFFER, counter->render.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(OPENGL_BASIC_VERTICES),
                     OPENGL_BASIC_VERTICES, GL_STATIC_DRAW);

        glBindVertexArray(counter->render.VAO);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                              2 * sizeof *OPENGL_BASIC_VERTICES, NULL);

        glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
        glBindVertexArray(GL_NONE);

        counter_update_x(counter, COUNTER_DEFAULT);
    }
    else {
        logger_fatal("Failed to allocate memory for counter");
    }

    return counter;
}

void counter_update_x(counter_t counter, float x)
{
    mat4 *cur = counter->models;
    size_t i;
    
    static const float ty = COUNTER_Y + COUNTER_OFFSET_Y;
    static const float sx = COUNTER_WIDTH;
    static const float sy = COUNTER_HEIGHT;

    for (i = 0; i < COUNTER_NUMBERS; ++i, ++cur) {
        matrix4x4_identity(*cur);
        matrix4x4_scale(*cur, sx, sy);
        matrix4x4_translate(*cur, x + i * sx, ty);
    }
}

void counter_render(const counter_t counter, mat4 projection)
{
    shader_t shader = resources_shader(RS_SHADER_COUNTER);
    int i, n;

    static long long n_max = -1;
    static long long p_base[COUNTER_NUMBERS];

    if (counter == NULL || projection == NULL) return;

    if (n_max < 0) {
        n_max = binary_pow(10, COUNTER_NUMBERS) - 1;
        
        for (i = 0; i < COUNTER_NUMBERS; i++) {
            p_base[i] = binary_pow(10, (COUNTER_NUMBERS - 1) - i);
        }
    }

    n = MAX(MIN(n_max, counter->value), 0);

    shader_use(shader);
    shader_set_uniform_m4fv(shader, "u_projection", projection);

    texture_bind(resources_texture_atlas());

    glBindVertexArray(counter->render.VAO);

    for (i = 0; i < COUNTER_NUMBERS; ++i) {
        shader_set_uniform_m4fv(shader, "u_model", counter->models[i]);
        shader_set_uniform_1i(shader, "u_number", n / p_base[i] % 10);
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

        free(counter->models);
        free(counter);
    }
}