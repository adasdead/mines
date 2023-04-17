#include "game/counter.h"

#include <string.h>

#include "util/resources.h"
#include "util/logger.h"

#include "definitions.h"

#define COUNTER_OFFSET                                      0.25f
#define COUNTER_SCALE_FACTOR_X                              0.82f
#define COUNTER_SCALE_FACTOR_Y                              1.465f

#define norm(x) ((x) < 999) ? (((x) > 0) ? (x) : 0) : 999

counter_t counter_create(void)
{
    counter_t counter = malloc(sizeof *counter);

    if (counter != NULL) {
        memset(counter, 0, sizeof *counter);

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

        counter_update_x(counter, 0);
    }
    else {
        logger_fatal("Failed to allocate memory for counter");
    }

    return counter;
}

void counter_update_x(counter_t counter, u32 x)
{
    i32 i;

    for (i = 0; i < 3; i++) {
        matrix4x4_free(counter->models[i]);
        counter->models[i] = matrix4x4_allocate(true);
        matrix4x4_scale(counter->models[i], COUNTER_SCALE_FACTOR_X,
                        COUNTER_SCALE_FACTOR_Y);
        matrix4x4_translate(counter->models[i],
                            x + COUNTER_OFFSET + i * COUNTER_SCALE_FACTOR_X,
                            COUNTER_Y + COUNTER_OFFSET);
    }
}

void counter_render(const counter_t counter, mat4 projection)
{
    shader_t shader = resources_shader(RS_SHADER_COUNTER);

    shader_use(shader);
    shader_set_uniform_m4fv(shader, "u_projection", projection);

    texture_bind(resources_texture_atlas());

    glBindVertexArray(counter->render.VAO);

    shader_set_uniform_m4fv(shader, "u_model", counter->models[0]);
    shader_set_uniform_1i(shader, "u_number", norm(counter->value / 100 % 10));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);

    shader_set_uniform_m4fv(shader, "u_model", counter->models[1]);
    shader_set_uniform_1i(shader, "u_number", norm(counter->value / 10 % 10));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);

    shader_set_uniform_m4fv(shader, "u_model", counter->models[2]);
    shader_set_uniform_1i(shader, "u_number", norm(counter->value % 10));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);

    glBindVertexArray(0);
}

void counter_free(counter_t counter)
{
    if (counter != NULL) {
        matrix4x4_free(counter->models[0]);
        matrix4x4_free(counter->models[1]);
        matrix4x4_free(counter->models[2]);

        free(counter);
    }
}