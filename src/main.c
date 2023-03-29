#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#include "graphics/shader.h"
#include "graphics/texture.h"

#include "math/projection.h"

#include "field.h"

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(480, 480, "Hello", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    GLuint VBO, VAO;

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    struct field *field = field_create(3, 3, 5);
    field_generate(field, 2, 2);

    u8 *value = (u8*) field->cells;

    for (int i = 0; i < field->width; i++) {
        for (int j = 0; j < field->height; j++) {
            printf("%d ", value[field->width * i + j]);
        }

        printf("\n");
    }

    static u8 info[] = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof info, info, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof *info, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    glBindVertexArray(GL_NONE);

    shader_t shader = shader_load_g("default.vs.glsl", "default.fs.glsl",
                                    "default.gs.glsl");

    struct texture *atlas = texture_load("atlas.png");

    m4x4 projection = projection_ortho(0, field->width, field->height, 0, -1, 1);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader_use(shader);
        shader_set_uniform_m4fv(shader, "projection", projection);
        shader_set_uniform_2i(shader, "mapSize", field->width, field->height);

        texture_bind(atlas);

        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, field->width * field->height);
        glBindVertexArray(GL_NONE);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shader_free(shader);
    texture_free(atlas);
    field_free(field);

    glfwTerminate();
    return 0;
}