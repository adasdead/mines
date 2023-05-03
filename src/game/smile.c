#include "game/smile.h"

#include <stdlib.h>
#include <stdio.h>

#include "game/game.h"
#include "game/window.h"

#include "util/resources.h"
#include "util/logger.h"

#include "definitions.h"

static bool check_cursor_on_smile(smile_t smile, uint x, uint y)
{
    float scale_factor = window_instance()->scale;
    float left = (smile->x + SMILE_OFFSET) * scale_factor;
    float top = (SMILE_LY + SMILE_OFFSET) * scale_factor;
    float offset = SMILE_SCALE * scale_factor;

    return !((int)(y <= top)  | (int)(y >= top + offset) |
             (int)(x <= left) | (int)(x >= left + offset));
}

smile_t smile_create(void (*click_callback)(void))
{
    smile_t smile = malloc(sizeof *smile);

    if (smile != NULL) {
        smile->model = matrix4x4_allocate();
        smile->click_callback = click_callback;

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

void smile_update_width(smile_t smile, uint width)
{
    static const float y = SMILE_LY + SMILE_OFFSET;
    
    smile->x = (width / 2.0f) - 1.0f;

    matrix4x4_identity(smile->model);
    matrix4x4_scale(smile->model, SMILE_SCALE, SMILE_SCALE);
    matrix4x4_translate(smile->model, smile->x + SMILE_OFFSET, y);
}

void smile_render(const smile_t smile, mat4 projection)
{
    shader_t shader = resources_shader(RS_SHADER_SMILE);

    if (smile == NULL || projection == NULL) return;

    shader_use(shader);
    shader_set_uniform_m4fv(shader, "u_projection", projection);
    shader_set_uniform_m4fv(shader, "u_model", smile->model);
    shader_set_uniform_1i(shader, "u_smile_state", smile->state);

    texture_bind(resources_texture_atlas());

    glBindVertexArray(smile->render.VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);
    glBindVertexArray(0);
}

void smile_mouse(smile_t smile, uint x, uint y, bool press)
{
    if (check_cursor_on_smile(smile, x, y)) {
        if (press) {
            smile->state = SMILE_STATE_CLICK;
            return;
        }

        smile->click_callback();
    }

    switch (smile->state) {
    case SMILE_STATE_DEAD:
    case SMILE_STATE_COOL:
        return;
    
    default:
        if (press) {
            smile->state = SMILE_STATE_O;
        } else {
            smile->state = SMILE_STATE_DEFAULT;
        }
    }
}

void smile_free(smile_t smile)
{
    if (smile != NULL) {
        glDeleteVertexArrays(1, &smile->render.VAO);
        glDeleteBuffers(1, &smile->render.VBO);
        matrix4x4_free(smile->model);
        free(smile);
    }
}