#ifndef GRAPHICS_OPENGL_H
#define GRAPHICS_OPENGL_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#define OPENGL_RENDER_STRUCT                                            \
    struct {                                                            \
        GLuint          VAO;                                            \
        GLuint          VBO;                                            \
    } render

#define OPENGL_RENDER_GEN_BASIC_BUFFERS(object)                         \
    glGenVertexArrays(1, &(object)->render.VAO);                        \
    glGenBuffers(1, &(object)->render.VBO);                             \
                                                                        \
    glBindBuffer(GL_ARRAY_BUFFER, (object)->render.VBO);                \
    glBufferData(GL_ARRAY_BUFFER, sizeof(OPENGL_BASIC_VERTICES),        \
                    OPENGL_BASIC_VERTICES, GL_STATIC_DRAW);             \
                                                                        \
    glBindVertexArray((object)->render.VAO);                            \
                                                                        \
    glEnableVertexAttribArray(0);                                       \
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,                     \
                          2 * sizeof *OPENGL_BASIC_VERTICES, NULL);     \
                                                                        \
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);                             \
    glBindVertexArray(GL_NONE);

#define OPENGL_RENDER_FREE(object)                                      \
    glDeleteVertexArrays(1, &(object)->render.VAO);                     \
    glDeleteBuffers(1, &(object)->render.VBO);

static const GLfloat OPENGL_BASIC_VERTICES[] = {
    0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
};

#endif /* GRAPHICS_OPENGL_H */