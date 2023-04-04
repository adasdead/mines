#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include "game/window.h"

#include "graphics/opengl.h"

#include "math/matrix4x4.h"

#include "util/basic_types.h"

#include "cell.h"

#define field_normalize_mouse_pos(field, x, y)                          \
    do {                                                                \
        window_normalize_mouse_pos(&x, &y);                             \
        x = (x - FIELD_LX >= field->width) ? -1 :  x - FIELD_LX;        \
        y = (y - FIELD_LY >= field->height) ? -1 : y - FIELD_LY;        \
    } while (0)

struct field {
    usize size;
    u32 width, height;
    u32 mines;

    union cell *cells;

    struct {
        GLuint VAO, VBO;
    } render;
};

typedef struct field *field_t;

field_t field_create(u32 width, u32 height, u32 mines);
cell_t field_cell(field_t field, i32 x, i32 y);
void field_generate(field_t field, u32 x, u32 y);
usize field_adjacent_mines(field_t field, u32 x, u32 y);
void field_render(field_t field, mat4 projection, i32 mouse_x, i32 mouse_y);
void field_free(field_t field);

#endif /* GAME_FIELD_H */