/* 
 * Copyright (c) 2023 adasdead
 * This software is licensed under the MIT License.
 */

#include "game/border.h"

#include <stdio.h>
#include <string.h>

#include "util/resources.h"
#include "util/logger.h"

#include "definitions.h"

enum border_titles {
    BORDER_TT_BACKGROUND,
    BORDER_TT_CORBER_NW,
    BORDER_TT_CORBER_NE,
    BORDER_TT_CORBER_SE,
    BORDER_TT_CORBER_SW,
    BORDER_TT_HORIZONTAL,
    BORDER_TT_SEP_RIGHT,
    BORDER_TT_VERTICAL,
    BORDER_TT_SEP_LEFT,
};

static void fill_frame(byte *frame, int width, int height)
{
    int x, y;

    /* Initialize the frame with the background title */
    memset(frame, BORDER_TT_BACKGROUND, width * height);

    /* Horizontal lines */
    for (x = 0; x < width; ++x) {
        frame[x] = BORDER_TT_HORIZONTAL;
        frame[width * BORDER_SEP_LEFT_Y + x] = BORDER_TT_HORIZONTAL;
        frame[width * (height - 1) + x] = BORDER_TT_HORIZONTAL;
    }

    /* Vertical lines */
    for (y = 0; y < height; ++y) {
        frame[width * y] = BORDER_TT_VERTICAL;
        frame[width * y + (width - 1)] = BORDER_TT_VERTICAL;
    }

    /* Corners */
    frame[0] = BORDER_TT_CORBER_NW;
    frame[width - 1] = BORDER_TT_CORBER_NE;
    frame[width * (height - 1)] = BORDER_TT_CORBER_SW;
    frame[width * height - 1] = BORDER_TT_CORBER_SE;

    /* Separators */
    frame[width * BORDER_SEP_LEFT_Y] = BORDER_TT_SEP_RIGHT;
    frame[width * BORDER_SEP_LEFT_Y + (width - 1)] = BORDER_TT_SEP_LEFT;
}

border_t border_create(void)
{
    border_t border = calloc(1, sizeof *border);

    if (border == NULL)
        logger_fatal("Failed to allocate memory for border");

    return border;
}

void border_update(border_t border, int width, int height)
{
    if (border == NULL) return;

    border->width = width;
    border->height = height;
    border->size = width * height;

    border->frame = realloc(border->frame, border->size);

    if (border->frame == NULL)
        logger_fatal("Failed to allocate memory for border frame");

    fill_frame(border->frame, width, height);

    renderer_free(&border->renderer);
    renderer_titles_initialize(&border->renderer,
                               border->width, border->height);

    logger_info("Border updated");
}

void border_render(const border_t border, mat4 projection)
{
    shader_t shader = resources_shader(RS_SHADER_BORDER);

    if (projection == NULL || border->frame == NULL) return;

    shader_use(shader);
    shader_set_uniform_m4fv(shader, "u_projection", projection);
    shader_set_uniform_2i(shader, "u_size", border->width,
                          border->height);
    
    texture_bind(resources_texture_atlas());

    renderer_titles_draw(&border->renderer, border->frame,
                         border->size);
}

void border_free(border_t border)
{
    if (border != NULL) {
        if (border->frame != NULL)
            free(border->frame);

        renderer_free(&border->renderer);
        free(border);
    }
}