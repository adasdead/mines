/* 
 * Copyright (c) 2023 adasdead
 * This software is licensed under the MIT License. (see the LICENSE file)
 */

#include "graphics/texture.h"

#include <stdlib.h>

#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "util/logger.h"

static GLubyte *read_image_data_from_file(const char *file_path,
                                          int *width, int *height)
{
    if (file_path == NULL) {
        logger_warn("File path is null");
        return NULL;
    }

    return stbi_load(file_path, width, height, NULL, STBI_rgb_alpha);
}

texture_t texture_load(const char *file_path)
{
    texture_t texture = malloc(sizeof *texture);
    GLubyte *bytes;

    if (texture == NULL)
        logger_fatal("Failed to allocate memory for texture");

    bytes = read_image_data_from_file(file_path,
                                      &texture->width,
                                      &texture->height);
    
    if (bytes == NULL) {
        logger_warn("Failed to load image \"%s\"", file_path);
        free(texture);
        return NULL;
    }

    glGenTextures(1, &texture->id);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width,
                 texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);

    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    stbi_image_free(bytes);

    return texture;
}

void texture_bind(const texture_t texture)
{
    if (texture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->id);
    }
}

void texture_free(texture_t texture)
{
    free(texture);
}