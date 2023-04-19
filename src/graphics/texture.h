#ifndef GRAPHICS_TEXTURE_H
#define GRAPHICS_TEXTURE_H

#include "graphics/opengl.h"

struct texture {
    GLuint              id;
    int                 width;
    int                 height;
};

typedef struct texture *texture_t;

texture_t texture_load(const char *file_path);
void texture_bind(const texture_t texture);
void texture_free(texture_t texture);

#endif /* GRAPHICS_TEXTURE_H */