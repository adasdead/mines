#include "field.h"

#include <stdlib.h>

#include "logger.h"

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

        return field->cells + (field->width * x + y);
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

void field_render(const struct field *field)
{
    
}

void field_free(struct field *field)
{
    if (field != NULL) {
        free(field->cells);
        free(field);
    }
}