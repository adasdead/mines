#ifndef FIELD_H
#define FIELD_H

#include "glad/glad.h"

#include "math/matrix4x4.h"
#include "util/basic_types.h"

#include "cell.h"

#define FIELD_X     1
#define FIELD_Y     1

struct field {
    u32 width, height;
    u32 mines;

    union cell *cells;

    struct {
        GLuint VAO, VBO;
    } render;
};

struct field *field_create(u32 width, u32 height, u32 mines);
union cell *field_cell(const struct field *field, i32 x, i32 y);
void field_generate(const struct field *field, u32 x, u32 y);
void field_render(const struct field *field, m4x4 projection);
void field_free(struct field *field);

#endif /* FIELD_H */