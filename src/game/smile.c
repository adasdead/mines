/* 
 * Copyright (c) 2023 adasdead
 * This software is licensed under the MIT License.
 */

#include "game/smile.h"

#include <stdlib.h>
#include <stdio.h>

#include "game/game.h"
#include "game/window.h"

#include "util/resources.h"
#include "util/logger.h"

#include "definitions.h"

/* WARNING: x and y must not be normalized */
static bool is_cursor_over_smile(smile_t smile, uint x, uint y)
{
    float left = DENORMALIZE(smile->x + SMILE_OFFSET);
    float top = DENORMALIZE(SMILE_LEFT_Y + SMILE_OFFSET);
    float scale = DENORMALIZE(SMILE_SCALE);

    return !(y <= top || y >= top + scale || x <= left ||
             x >= left + scale);
}

smile_t smile_create(void (*click_callback)(void))
{
    smile_t smile = malloc(sizeof *smile);

    if (smile == NULL)
        logger_fatal("Failed to allocate memory for smile");

    smile->model = matrix4x4_allocate();
    smile->click_callback = click_callback;

    renderer_basic_initialize(&smile->renderer);

    return smile;
}

void smile_update_model_matrix(smile_t smile)
{
    float x;

    static const float y = SMILE_LEFT_Y + SMILE_OFFSET;

    if (smile != NULL) {
        x = smile->x + SMILE_OFFSET;

        matrix4x4_identity(smile->model);
        matrix4x4_scale(smile->model, SMILE_SCALE, SMILE_SCALE);
        matrix4x4_translate(smile->model, x, y);
    }
}

void smile_render(const smile_t smile, mat4 projection)
{
    shader_t shader = resources_shader(RS_SHADER_SMILE);

    if (smile == NULL || projection == NULL)
        return;

    shader_use(shader);
    shader_set_uniform_m4fv(shader, "u_projection", projection);
    shader_set_uniform_m4fv(shader, "u_model", smile->model);
    shader_set_uniform_1i(shader, "u_smile_state", smile->state);

    texture_bind(resources_texture_atlas());

    glBindVertexArray(smile->renderer.VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);
    glBindVertexArray(0);
}

void smile_handle_mouse(smile_t smile, uint x, uint y, bool press)
{
    if (smile == NULL) return;

    if (is_cursor_over_smile(smile, x, y)) {
        if (press == true) {
            smile->state = SMILE_STATE_CLICK;
            return;
        }
        else
            smile->click_callback();
    }

    switch (smile->state) {
        case SMILE_STATE_DEAD:
        case SMILE_STATE_COOL:
            return;
        
        default:
            if (press == true)
                smile->state = SMILE_STATE_O;
            else
                smile->state = SMILE_STATE_DEFAULT;
    }
}

void smile_free(smile_t smile)
{
    if (smile != NULL) {
        renderer_free(&smile->renderer);
        matrix4x4_free(smile->model);
        free(smile);
    }
}