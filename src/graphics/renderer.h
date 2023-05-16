/* 
 * Copyright (c) 2023 adasdead
 * This software is licensed under the MIT License. (see the LICENSE file)
 */

#ifndef GRAPHICS_RENDERER_H
#define GRAPHICS_RENDERER_H

#include "graphics/opengl.h"

#include "util/basic_types.h"

struct renderer {
    GLuint          VAO;
    GLuint          VBO;
};

void renderer_basic_initialize(struct renderer *renderer);
void renderer_titles_initialize(struct renderer *renderer,
                                uint width, uint height);
            
void renderer_titles_draw(const struct renderer *renderer,
                          const byte *titles, size_t size);

void renderer_free(struct renderer *renderer);

#endif /* GRAPHICS_RENDERER_H */