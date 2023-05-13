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

static void border_recreate_buffers(border_t border)
{
    size_t new_size = border->width * border->height;

    glDeleteVertexArrays(1, &border->render.VAO);
    glDeleteBuffers(1, &border->render.VBO);

    glGenVertexArrays(1, &border->render.VAO);
    glGenBuffers(1, &border->render.VBO);

    glBindBuffer(GL_ARRAY_BUFFER, border->render.VBO);
    glBufferData(GL_ARRAY_BUFFER, new_size, NULL, GL_STATIC_DRAW);

    glBindVertexArray(border->render.VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribIPointer(0, 1, GL_UNSIGNED_BYTE, sizeof(GLubyte), NULL);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    glBindVertexArray(GL_NONE);

    logger_info("Border buffers recreated");
}

border_t border_create(void)
{
    border_t border = calloc(1, sizeof *border);

    if (border == NULL) {
        logger_fatal("Failed to allocate memory for border");
    }

    return border;
}

void border_update_frame(border_t border, int width, int height)
{
    size_t new_frame_size = width * height * sizeof *border->frame;
    size_t x, y;

    if (border == NULL) return;

    border->frame = realloc(border->frame, new_frame_size);

    if (border->frame == NULL) {
        logger_fatal("Failed to allocate memory for border frame");
    }

    /* Initialize the frame with the background title */
    memset(border->frame, BORDER_TT_BACKGROUND, new_frame_size);

    /* Draw the horizontal lines */
    for (x = 0; x < width; ++x) {
        border->frame[x] = BORDER_TT_HORIZONTAL;
        border->frame[width * BORDER_SEP_LY + x] = BORDER_TT_HORIZONTAL;
        border->frame[width * (height - 1) + x] = BORDER_TT_HORIZONTAL;
    }

    /* Draw the vertical lines */
    for (y = 0; y < height; ++y) {
        border->frame[width * y] = BORDER_TT_VERTICAL;
        border->frame[width * y + (width - 1)] = BORDER_TT_VERTICAL;
    }

    /* Draw the corners */
    border->frame[0] = BORDER_TT_CORBER_NW;
    border->frame[width - 1] = BORDER_TT_CORBER_NE;
    border->frame[width * (height - 1)] = BORDER_TT_CORBER_SW;
    border->frame[width * height - 1] = BORDER_TT_CORBER_SE;

    /* Draw the separators */
    border->frame[width * BORDER_SEP_LY] = BORDER_TT_SEP_LEFT;
    border->frame[width * BORDER_SEP_LY + (width - 1)] = BORDER_TT_SEP_RIGHT;

    border->width = width;
    border->height = height;

    border_recreate_buffers(border);

    logger_info("Border frame updated");
}

void border_render(const border_t border, mat4 projection)
{
    size_t size = border->width * border->height * sizeof(GLubyte);
    shader_t shader = resources_shader(RS_SHADER_BORDER);

    if (projection == NULL || border->frame == NULL) return;

    shader_use(shader);
    shader_set_uniform_m4fv(shader, "u_projection", projection);
    shader_set_uniform_2i(shader, "u_size", border->width, border->height);
    texture_bind(resources_texture_atlas());

    glBindBuffer(GL_ARRAY_BUFFER, border->render.VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, border->frame);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindVertexArray(border->render.VAO);
    glDrawArrays(GL_POINTS, 0, size);
    glBindVertexArray(GL_NONE);
}

void border_free(border_t border)
{
    if (border != NULL) {
        if (border->frame != NULL) {
            free(border->frame);
        }

        OPENGL_RENDER_FREE(border);
        free(border);
    }
}