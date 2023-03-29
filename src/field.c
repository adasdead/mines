#include "field.h"

#include <stdlib.h>
#include <string.h>

#include "util/resources.h"
#include "util/logger.h"

static usize field_get_adjacent_mines(const struct field *field,
                                      u32 x, u32 y)
{
    usize mines = 0;
    i32 off_x, off_y;

    for (off_x = -1; off_x <= 1; off_x++) {
        for (off_y = -1; off_y <= 1; off_y++) {
            if (off_x == 0 && off_y == 0) {
                continue;
            }
            
            union cell *cell = field_cell(field, x + off_x, y + off_y);

            if (cell != NULL) {
                mines += (cell->type == CELL_TYPE_BOMB);
            }
        }
    }

    return mines;
}

struct field *field_create(u32 width, u32 height, u32 mines)
{
    struct field *field = malloc(sizeof *field);

    if (field != NULL) {
        field->height = height;
        field->width = width;
        field->mines = mines;

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

        logger_info("Field(%zux%zu | %zu mines) created!", width,
                    height, mines);
    }
    else {
        logger_fatal("Failed to allocate memory for field");
    }

ret:
    return field;
}

union cell *field_cell(const struct field *field, i32 x, i32 y)
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

void field_generate(const struct field *field, u32 x, u32 y)
{
    usize placed_mines = 0;
    u32 rnd_x, rnd_y;

    if (field == NULL) {
        logger_warn("field is NULL (field_generate)");
        return;
    }

    memset(field->cells, 0, field->width * field->height * sizeof(u8));

    while (placed_mines != field->mines) {
        rnd_x = rand() % field->width;
        rnd_y = rand() % field->height;

        if (rnd_x == x && rnd_y == y) {
            continue;
        }

        union cell *cell = field_cell(field, rnd_x, rnd_y);

        if (cell->type != CELL_TYPE_BOMB) {
            cell->type = CELL_TYPE_BOMB;
            placed_mines++;
        }
    }

    for (x = 0; x < field->width; x++) {
        for (y = 0; y < field->height; y++) {
            union cell *cell = field_cell(field, x, y);

            if (cell->type != CELL_TYPE_BOMB) {
                cell->type = field_get_adjacent_mines(field, x, y);
            }
        }
    }

    logger_info("Field(%zux%zu | %zu mines) generated",
                field->width, field->height, field->mines);
}

void field_render(const struct field *field, m4x4 projection)
{
    shader_t shader = resources_shader(RS_SHADER_FIELD);
    usize size = field->width * field->height * sizeof(u8);

    shader_use(shader);
    shader_set_uniform_m4fv(shader, "u_projection", projection);
    shader_set_uniform_2i(shader, "u_field_size", field->width, field->height);
    shader_set_uniform_2i(shader, "u_field_pos", FIELD_X, FIELD_Y);

    texture_bind(resources_texture_atlas());

    glBindBuffer(GL_ARRAY_BUFFER, field->render.VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, field->cells);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindVertexArray(field->render.VAO);
    glDrawArrays(GL_POINTS, 0, size);
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