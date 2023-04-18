#include "game/counter.h"

#include <string.h>

#include "util/resources.h"
#include "util/logger.h"

#include "definitions.h"

#define OFFSET_X                                    0.25f
#define OFFSET_Y                                    OFFSET_X
#define SCALE_FACTOR_X                              0.82f
#define SCALE_FACTOR_Y                              1.465f
#define DEFAULT                                     1

/***************************************************************************/

/* https://cp-algorithms.com/algebra/binary-exp.html#implementation */
static long long binary_pow(long long n, long long x)
{
    long long result = 1;

    while (x != 0) {
        if(x & 1) result *= n;

        n *= n;
        x >>= 1;
    }

    return result;
}

/***************************************************************************/

counter_t counter_create(void)
{
    counter_t counter = malloc(sizeof *counter);

    if (counter != NULL) {
        memset(counter, 0, sizeof *counter);

        counter->models = calloc(COUNTER_NUMBERS,
                                 sizeof *counter->models);

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

        counter_update_x(counter, DEFAULT);
    }
    else {
        logger_fatal("Failed to allocate memory for counter");
    }

    return counter;
}

void counter_update_x(counter_t counter, uint x)
{
    int i;
    float tx;

    for (i = 0; i < COUNTER_NUMBERS; ++i) {
        tx = x + OFFSET_X + i * SCALE_FACTOR_X;
        matrix4x4_free(counter->models[i]);
        counter->models[i] = matrix4x4_allocate(true);
        matrix4x4_scale(counter->models[i], SCALE_FACTOR_X, SCALE_FACTOR_Y);
        matrix4x4_translate(counter->models[i], tx, COUNTER_Y + OFFSET_Y);
    }
}

void counter_render(const counter_t counter, mat4 projection)
{
    int i, p, n = binary_pow(10, COUNTER_NUMBERS) - 1;
    shader_t shader;

    shader_use(shader = resources_shader(RS_SHADER_COUNTER));
    shader_set_uniform_m4fv(shader, "u_projection", projection);

    texture_bind(resources_texture_atlas());

    glBindVertexArray(counter->render.VAO);

    if (n > counter->value) n = counter->value;
    if (0 > counter->value) n = 0;

    for (int i = 0; i < COUNTER_NUMBERS; ++i) {
        p = binary_pow(10, (COUNTER_NUMBERS - 1) - i);

        shader_set_uniform_m4fv(shader, "u_model", counter->models[i]);
        shader_set_uniform_1i(shader, "u_number", n / p % 10);
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