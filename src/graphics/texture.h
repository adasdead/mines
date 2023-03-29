#ifndef GRAPHICS_TEXTURE_H
#define GRAPHICS_TEXTURE_H

#include "glad/glad.h"

#include "basic_types.h"

struct texture {
    GLuint  id;
    i32     width, height;
};

struct texture *texture_load(const char *file_path);
void texture_bind(const struct texture *texture);
void texture_free(struct texture *texture);

#endif /* GRAPHICS_TEXTURE_H */