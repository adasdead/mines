/* 
 * Copyright (c) 2023 adasdead
 * This software is licensed under the MIT License.
 */

#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include "graphics/renderer.h"

#include "game/window.h"
#include "game/cell.h"

#include "util/matrix4x4.h"
#include "util/basic_types.h"

#include "settings/definitions.h"

struct field {
    /* The total number of cells in the grid */
    size_t              size;

    uint                width;
    uint                height;
    uint                mines;

    struct cell        *cells;

    struct renderer     renderer;
};

typedef struct field *field_t;

static inline void field_shift_pos(int *x, int *y)
{
    *x -= FIELD_LEFT_X;
    *y -= FIELD_LEFT_Y;
}

static inline bool field_is_within(const field_t field,
                                   int x, int y)
{
    return (x >= 0) && (x < (int) field->width) &&
           (y >= 0) && (y < (int) field->height);
}

field_t field_create(uint width, uint height, uint mines);
cell_t field_cell(field_t field, int x, int y);
void field_generate(field_t field, uint x, uint y);
size_t field_adjacent_mines(field_t field, uint x, uint y);
void field_render(field_t field, mat4 projection);
void field_clear(field_t field);
void field_free(field_t field);

#endif /* GAME_FIELD_H */