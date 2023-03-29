#include "resources.h"

static struct texture *texture_atlas;
static shader_t shaders[RS_SHADER_TOTAL];

void resources_load(void)
{
    texture_atlas = texture_load("atlas.png");

    shaders[RS_SHADER_FIELD] = shader_load_g("field.vs.glsl",
                                             "field.fs.glsl",
                                             "field.gs.glsl");
}

struct texture *resources_texture_atlas(void)
{
    return texture_atlas;
}

shader_t resources_shader(enum resources_shader_type type)
{
    return shaders[(usize) type];
}

void resources_free(void)
{
    int i;

    for (i = 0; i < RS_SHADER_TOTAL; i++) {
        shader_free(shaders[i]);
    }

    texture_free(texture_atlas);
}