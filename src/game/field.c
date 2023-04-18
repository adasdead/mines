#include "field.h"

#include <stdlib.h>
#include <string.h>

#include "game/window.h"

#include "util/resources.h"
#include "util/logger.h"

#include "definitions.h"

field_t field_create(uint width, uint height, uint mines)
{
    field_t field = malloc(sizeof *field);

    if (field != NULL) {
        field->height = height;
        field->width = width;
        field->mines = mines;
        field->size = width * height;
        field->cells = calloc(width * height, sizeof *(field->cells));

        glGenBuffers(1, &field->render.VBO);
        glGenVertexArrays(1, &field->render.VAO);

        glBindBuffer(GL_ARRAY_BUFFER, field->render.VBO);
        glBufferData(GL_ARRAY_BUFFER, width * height, NULL, GL_STATIC_DRAW);

        glBindVertexArray(field->render.VAO);
        glEnableVertexAttribArray(0);
        glVertexAttribIPointer(0, 1, GL_UNSIGNED_BYTE, sizeof(uint8_t), NULL);
        glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
        glBindVertexArray(GL_NONE);

        logger_info("Field(%ux%u | %u mines) created!", width,
                    height, mines);
    }
    else {
        logger_fatal("Failed to allocate memory for field");
    }

    return field;
}

bool field_normalize_pos(const field_t field, int *x, int *y)
{
    *x = ((int) (*x / CELL_WIDTH_PX / window_scale_factor())) - FIELD_LX;
    *y = ((int) (*y / CELL_WIDTH_PX / window_scale_factor())) - FIELD_LY;

    return (*x < field->width) && (*y < field->height) &&
           (*x >= 0)           && (*y >= 0);
}

cell_t field_cell(field_t field, int x, int y)
{
    if (field != NULL) {
        if (x >= field->width  || x < 0 ||
            y >= field->height || y < 0) {
            
            return NULL;
        }

        return field->cells + (field->width * y + x);
    }

    logger_warn("field is NULL (field_cell)");
    return NULL;
}

void field_generate(field_t field, uint x, uint y)
{
    cell_t cell;
    size_t placed_mines = 0;
    uint rnd_x, rnd_y;

    if (field == NULL) {
        logger_warn("field is NULL (field_generate)");
        return;
    }

    while (placed_mines != field->mines) {
        rnd_x = rand() % field->width;
        rnd_y = rand() % field->height;

        if (rnd_x == x && rnd_y == y) {
            continue;
        }

        cell = field_cell(field, rnd_x, rnd_y);

        if (cell->type != CELL_TYPE_BOMB) {
            cell->type = CELL_TYPE_BOMB;
            placed_mines++;
        }
    }

    for (x = 0; x < field->width; ++x) {
        for (y = 0; y < field->height; ++y) {
            cell = field_cell(field, x, y);

            if (cell->type != CELL_TYPE_BOMB) {
                cell->type = field_adjacent_mines(field, x, y);
            }
        }
    }

    logger_info("Field(%ux%u | %u mines) generated",
                field->width, field->height, field->mines);
}

size_t field_adjacent_mines(field_t field, uint x, uint y)
{
    cell_t cell;
    size_t mines = 0;
    int off_x, off_y;

    for (off_x = -1; off_x <= 1; ++off_x) {
        for (off_y = -1; off_y <= 1; ++off_y) {
            if (off_x == 0 && off_y == 0) {
                continue;
            }
            
            cell = field_cell(field, x + off_x, y + off_y);

            if (cell != NULL) {
                mines += (cell->type == CELL_TYPE_BOMB);
            }
        }
    }

    return mines;
}

void field_render(field_t field, mat4 projection)
{
    shader_t shader;
    int cur_x, cur_y;

    window_cursor_pos(&cur_x, &cur_y);
    field_normalize_pos(field, &cur_x, &cur_y);

    shader_use(shader = resources_shader(RS_SHADER_FIELD));

    shader_set_uniform_m4fv(shader, "u_projection", projection);
    shader_set_uniform_2i(shader, "u_field_size", field->width, field->height);
    shader_set_uniform_2i(shader, "u_field_pos", FIELD_LX, FIELD_LY);
    shader_set_uniform_2i(shader, "u_mouse_pos", cur_x, cur_y);

    texture_bind(resources_texture_atlas());

    glBindBuffer(GL_ARRAY_BUFFER, field->render.VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, field->size * sizeof(uint8_t),
                    field->cells);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindVertexArray(field->render.VAO);
    glDrawArrays(GL_POINTS, 0, field->size);
    glBindVertexArray(GL_NONE);
}

void field_free(struct field *field)
{
    if (field != NULL) {
        glDeleteVertexArrays(1, &field->render.VAO);
        glDeleteBuffers(1, &field->render.VBO);
        free(field->cells);
        free(field);
    }
}