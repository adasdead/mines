#include "field.h"

#include <stdlib.h>
#include <string.h>

#include "game/window.h"

#include "util/resources.h"
#include "util/logger.h"

#include "definitions.h"

field_t field_create(u32 width, u32 height, u32 mines)
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
        glVertexAttribIPointer(0, 1, GL_UNSIGNED_BYTE, sizeof(u8), NULL);
        glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
        glBindVertexArray(GL_NONE);

        logger_info("Field(%ux%u | %u mines) created!", width,
                    height, mines);
    }
    else {
        logger_fatal("Failed to allocate memory for field");
    }

ret:
    return field;
}

cell_t field_cell(field_t field, i32 x, i32 y)
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

void field_generate(field_t field, u32 x, u32 y)
{
    cell_t cell;
    usize placed_mines = 0;
    u32 rnd_x, rnd_y;

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

    for (x = 0; x < field->width; x++) {
        for (y = 0; y < field->height; y++) {
            cell = field_cell(field, x, y);

            if (cell->type != CELL_TYPE_BOMB) {
                cell->type = field_adjacent_mines(field, x, y);
            }
        }
    }

    logger_info("Field(%ux%u | %u mines) generated",
                field->width, field->height, field->mines);
}

usize field_adjacent_mines(field_t field, u32 x, u32 y)
{
    cell_t cell;
    usize mines = 0;
    i32 off_x, off_y;

    for (off_x = -1; off_x <= 1; off_x++) {
        for (off_y = -1; off_y <= 1; off_y++) {
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

void field_render(field_t field, mat4 projection, i32 mouse_x, i32 mouse_y)
{
    shader_t shader = resources_shader(RS_SHADER_FIELD);

    field_normalize_mouse_pos(field, mouse_x, mouse_y);

    shader_use(shader);

    shader_set_uniform_m4fv(shader, "u_projection", projection);
    shader_set_uniform_2i(shader, "u_field_size", field->width, field->height);
    shader_set_uniform_2i(shader, "u_field_pos", FIELD_LX, FIELD_LY);
    shader_set_uniform_2i(shader, "u_mouse_pos", mouse_x, mouse_y);

    texture_bind(resources_texture_atlas());

    glBindBuffer(GL_ARRAY_BUFFER, field->render.VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, field->size * sizeof(u8), field->cells);
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