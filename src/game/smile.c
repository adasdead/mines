#include "game/smile.h"

#include <stdlib.h>
#include <stdio.h>

#include "game/game.h"
#include "game/window.h"

#include "util/resources.h"
#include "util/logger.h"

#include "definitions.h"

#define SMILE_OFFSET                                        0.15f
#define SMILE_SCALE_FACTOR                                  1.655f

static bool check_cursor_on_smile(smile_t smile, u32 x, u32 y)
{
    f32 cell_width_px = window_factor() * CELL_WIDTH_PX;
    f32 left = (smile->x + SMILE_OFFSET) * cell_width_px;
    f32 top = (SMILE_Y + SMILE_OFFSET) * cell_width_px;
    f32 offset = SMILE_SCALE_FACTOR * cell_width_px;

    return !(y <= top) && !(y >= top + offset) &&
           !(x <= left) && !(x >= left + offset);
}

smile_t smile_create(enum smile_state state)
{
    smile_t smile = malloc(sizeof *smile);

    if (smile != NULL) {
        smile->state = state;
        smile->model = matrix4x4_allocate(false);

        glGenVertexArrays(1, &smile->render.VAO);
        glGenBuffers(1, &smile->render.VBO);

        glBindBuffer(GL_ARRAY_BUFFER, smile->render.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(OPENGL_BASIC_VERTICES),
                     OPENGL_BASIC_VERTICES, GL_STATIC_DRAW);

        glBindVertexArray(smile->render.VAO);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                              2 * sizeof *OPENGL_BASIC_VERTICES, NULL);

        glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
        glBindVertexArray(GL_NONE);

    }
    else {
        logger_fatal("Failed to allocate memory for smile");
    }

    return smile;
}

void smile_update_width(smile_t smile, u32 field_width)
{
    smile->x = ((FIELD_LX + FIELD_RX + field_width) / 2.0f) - 1.0f;

    matrix4x4_free(smile->model);
    smile->model = matrix4x4_allocate(true);
    matrix4x4_scale(smile->model, SMILE_SCALE_FACTOR, SMILE_SCALE_FACTOR);
    matrix4x4_translate(smile->model, smile->x + SMILE_OFFSET,
                        SMILE_Y + SMILE_OFFSET);
}

void smile_render(const smile_t smile, mat4 projection)
{
    shader_t shader = resources_shader(RS_SHADER_SMILE);

    shader_use(shader);
    shader_set_uniform_m4fv(shader, "u_projection", projection);
    shader_set_uniform_m4fv(shader, "u_model", smile->model);
    shader_set_uniform_1i(shader, "u_smile_state", smile->state);

    texture_bind(resources_texture_atlas());

    glBindVertexArray(smile->render.VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);
    glBindVertexArray(0);
}

void smile_mouse(smile_t smile, u32 x, u32 y, bool press)
{
    if (check_cursor_on_smile(smile, x, y)) {
        if (press) {
            smile->state = SMILE_STATE_CLICK;
            return;
        }
        else {
            game_new();
        }
    }

    if (smile->state == SMILE_STATE_DEAD) return;
    if (smile->state == SMILE_STATE_COOL) return;

    if (press) {
        smile->state = SMILE_STATE_O;
    }
    else {
        smile->state = SMILE_STATE_DEFAULT;
    }
}

void smile_free(smile_t smile)
{
    if (smile != NULL) {
        glDeleteVertexArrays(1, &smile->render.VAO);
        glDeleteBuffers(1, &smile->render.VBO);
        free(smile);
    }
}