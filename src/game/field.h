#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include "graphics/opengl.h"

#include "game/window.h"
#include "game/cell.h"

#include "util/matrix4x4.h"
#include "util/basic_types.h"

#include "definitions.h"

struct field {
    size_t              size;

    uint                width;
    uint                height;
    uint                mines;

    struct cell         *cells;

    struct {
        GLuint          VAO;
        GLuint          VBO;
    } render;
};

typedef struct field *field_t;

field_t field_create(uint width, uint height, uint mines);
bool field_normalize_pos(const field_t field, int *x, int *y);
cell_t field_cell(field_t field, int x, int y);
void field_generate(field_t field, uint x, uint y);
size_t field_adjacent_mines(field_t field, uint x, uint y);
void field_render(field_t field, mat4 projection);
void field_free(field_t field);

#endif /* GAME_FIELD_H */