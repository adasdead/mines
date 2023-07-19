/* 
 * Copyright (c) 2023 adasdead
 * This software is licensed under the MIT License.
 */

#include "resources.h"

#include <stb/stb_image.h>

#include "settings/definitions.h"

static texture_t texture_atlas;
static shader_t shaders[RS_SHADER_TOTAL];

void resources_load(void)
{
    texture_atlas = texture_load(ASSETS_DIR"/atlas.png");

    shaders[RS_SHADER_FIELD] = shader_load_g(ASSETS_DIR"/field.vs.glsl",
                                             ASSETS_DIR"/field.fs.glsl",
                                             ASSETS_DIR"/field.gs.glsl");

    shaders[RS_SHADER_SMILE] = shader_load(ASSETS_DIR"/base.vs.glsl",
                                           ASSETS_DIR"/smile.fs.glsl");

    shaders[RS_SHADER_COUNTER] = shader_load(ASSETS_DIR"/base.vs.glsl",
                                             ASSETS_DIR"/counter.fs.glsl");

    shaders[RS_SHADER_BORDER] = shader_load_g(ASSETS_DIR"/border.vs.glsl",
                                              ASSETS_DIR"/border.fs.glsl",
                                              ASSETS_DIR"/border.gs.glsl");
}

shader_t resources_shader(enum resources_shader_type type)
{
    return shaders[(size_t) type];
}

texture_t resources_texture_atlas(void)
{
    return texture_atlas;
}

void resources_free(void)
{
    int i;

    for (i = 0; i < RS_SHADER_TOTAL; ++i) {
        shader_free(shaders[i]);
    }

    texture_free(texture_atlas);
}