#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include "graphics/opengl.h"

#include "game/window.h"
#include "game/cell.h"

#include "util/matrix4x4.h"
#include "util/basic_types.h"

#include "definitions.h"

struct field {
    /* The total number of cells in the grid */
    size_t              size;

    uint                width;
    uint                height;
    uint                mines;

    struct cell         *cells;

    OPENGL_RENDER_STRUCT;
};

typedef struct field *field_t;

static inline void field_normalize_pos(int *x, int *y)
{
    if (x == NULL || y == NULL) return;

    window_normalize_pos(x, y);

    *x -= FIELD_LX;
    *y -= FIELD_LY;
}

field_t field_create(uint width, uint height, uint mines);
bool field_is_include(const field_t field, int x, int y);
cell_t field_cell(field_t field, int x, int y);
void field_generate(field_t field, uint x, uint y);
size_t field_adjacent_mines(field_t field, uint x, uint y);
void field_render(field_t field, mat4 projection);
void field_clear(field_t field);
void field_free(field_t field);

#endif /* GAME_FIELD_H */