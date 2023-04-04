#include "game/smile.h"

#include <stdlib.h>
#include <stdio.h>

#include "game/game.h"
#include "game/window.h"

#include "util/resources.h"
#include "util/logger.h"

#include "definitions.h"

static GLfloat smile_vertices[] = {
    0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
};

static bool check_cursor_on_smile(smile_t smile, u32 x, u32 y)
{
    float cell_width_px = window_factor() * CELL_WIDTH_PX;
    float left = (smile->x + 0.15f) * cell_width_px;
    float top = (SMILE_Y + 0.15f) * cell_width_px;
    float offset = 1.65f * cell_width_px;

    if (y <= top) return false;
    if (y >= top + offset) return false;
    if (x <= left) return false;
    if (x >= left + offset) return false;

    return true;
}

smile_t smile_create(enum smile_state state)
{
    smile_t smile = malloc(sizeof *smile);

    if (smile != NULL) {
        smile->state = state;

        glGenVertexArrays(1, &smile->render.VAO);
        glGenBuffers(1, &smile->render.VBO);

        glBindBuffer(GL_ARRAY_BUFFER, smile->render.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(smile_vertices),
                     smile_vertices, GL_STATIC_DRAW);

        glBindVertexArray(smile->render.VAO);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                              2 * sizeof *smile_vertices, NULL);

        glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
        glBindVertexArray(GL_NONE);

    }
    else {
        logger_fatal("Failed to allocate memory for smile");
    }

    return smile;
}

void smile_set_field_width(smile_t smile, u32 field_width)
{
    smile->x = ((FIELD_LX + FIELD_RX + field_width) / 2.0f) - 1.0f;
}

void smile_render(const smile_t smile, mat4 projection)
{
    shader_t shader = resources_shader(RS_SHADER_SMILE);

    shader_use(shader);
    shader_set_uniform_m4fv(shader, "u_projection", projection);
    shader_set_uniform_2f(shader, "u_smile_pos", smile->x, SMILE_Y);
    shader_set_uniform_1i(shader, "u_smile_state", smile->state);

    texture_bind(resources_texture_atlas());

    glBindVertexArray(smile->render.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
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