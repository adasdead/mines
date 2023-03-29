#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#include "graphics/shader.h"
#include "graphics/texture.h"

#include "util/resources.h"

#include "math/projection.h"

#include "field.h"

static struct field *field;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double x, y;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwGetCursorPos(window, &x, &y);

        union cell *cell = field_cell(field, ((int) x - 60) / 60,
                                             ((int) y - 60) / 60);

        cell->state = CELL_STATE_OPENED;
    }
}

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
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    resources_load();
    
    field = field_create(6, 6, 5);
    field_generate(field, 2, 2);

    m4x4 projection = projection_ortho(0, field->width + 2,
                                       field->height + 2, 0, -1, 1);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        field_render(field, projection);

        glfwSwapBuffers(window);
        glfwWaitEvents();
    }

    field_free(field);
    resources_free();

    glfwTerminate();
    return 0;
}