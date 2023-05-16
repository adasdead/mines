/* 
 * Copyright (c) 2023 adasdead
 * This software is licensed under the MIT License. (see the LICENSE file)
 */

#include "graphics/renderer.h"

static const GLfloat BASIC_VERTICES[] = {
    0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
};

void renderer_basic_initialize(struct renderer *renderer)
{
    glGenVertexArrays(1, &renderer->VAO);
    glGenBuffers(1, &renderer->VBO);

    glBindBuffer(GL_ARRAY_BUFFER, renderer->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(BASIC_VERTICES),
                 BASIC_VERTICES, GL_STATIC_DRAW);

    glBindVertexArray(renderer->VAO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          2 * sizeof *BASIC_VERTICES, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    glBindVertexArray(GL_NONE);
}

void renderer_titles_initialize(struct renderer *renderer,
                                uint width, uint height)
{
    uint size = width * height;

    glGenBuffers(1, &renderer->VBO);
    glGenVertexArrays(1, &renderer->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, renderer->VBO);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);

    glBindVertexArray(renderer->VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribIPointer(0, 1, GL_UNSIGNED_BYTE, sizeof(GLubyte),
                           NULL);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    glBindVertexArray(GL_NONE);
}

void renderer_titles_draw(const struct renderer *renderer,
                          const byte *titles, size_t size)
{
    glBindBuffer(GL_ARRAY_BUFFER, renderer->VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, titles);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindVertexArray(renderer->VAO);
    glDrawArrays(GL_POINTS, 0, size);
    glBindVertexArray(GL_NONE);
}

void renderer_free(struct renderer *renderer)
{
    if (renderer != NULL) {
        glDeleteVertexArrays(1, &renderer->VAO);
        glDeleteBuffers(1, &renderer->VBO);
    }
}