#include "resources.h"

#include "definitions.h"

static texture_t texture_atlas;
static shader_t shaders[RS_SHADER_TOTAL];

void resources_load(void)
{
    texture_atlas = texture_load(ASSETS_DIR"atlas.png");

    shaders[RS_SHADER_FIELD] = shader_load_g(ASSETS_DIR"field.vs.glsl",
                                             ASSETS_DIR"field.fs.glsl",
                                             ASSETS_DIR"field.gs.glsl");

    shaders[RS_SHADER_SMILE] = shader_load(ASSETS_DIR"base.vs.glsl",
                                           ASSETS_DIR"smile.fs.glsl");

    shaders[RS_SHADER_COUNTER] = shader_load(ASSETS_DIR"base.vs.glsl",
                                             ASSETS_DIR"counter.fs.glsl");
}

texture_t resources_texture_atlas(void)
{
    return texture_atlas;
}

shader_t resources_shader(enum resources_shader_type type)
{
    return shaders[(size_t) type];
}

void resources_free(void)
{
    int i;

    for (i = 0; i < RS_SHADER_TOTAL; ++i) {
        shader_free(shaders[i]);
    }

    texture_free(texture_atlas);
}