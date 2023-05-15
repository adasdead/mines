#ifndef UTIL_RESOURCES_H
#define UTIL_RESOURCES_H

#include "graphics/shader.h"
#include "graphics/texture.h"

enum resources_shader_type {
    RS_SHADER_FIELD,
    RS_SHADER_SMILE,
    RS_SHADER_COUNTER,
    RS_SHADER_BORDER,
    RS_SHADER_TOTAL
};

void resources_load(void);
shader_t resources_shader(enum resources_shader_type type);
texture_t resources_texture_atlas(void);
void resources_free(void);

#endif /* UTIL_RESOURCES_H */