#ifndef FIELD_H
#define FIELD_H

#include "basic_types.h"
#include "cell.h"

struct field {
    u32 width, height;
    union cell *cells;
    u32 mines;
};

struct field *field_create(u32 width, u32 height, u32 mines);
union cell *field_cell(const struct field *field, i32 x, i32 y);
void field_generate(const struct field *field, u32 x, u32 y);
void field_render(const struct field *field);
void field_free(struct field *field);

#endif /* FIELD_H */