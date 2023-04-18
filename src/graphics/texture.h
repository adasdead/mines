#ifndef GRAPHICS_TEXTURE_H
#define GRAPHICS_TEXTURE_H

#include "graphics/opengl.h"

struct texture {
    GLuint              id;
    int                 width;
    int                 height;
};

struct texture *texture_load(const char *file_path);
void texture_bind(const struct texture *texture);
void texture_free(struct texture *texture);

#endif /* GRAPHICS_TEXTURE_H */