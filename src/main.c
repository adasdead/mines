/* 
 * Copyright (c) 2023 adasdead
 * This software is licensed under the MIT License. (see the LICENSE file)
 */

#include "graphics/opengl.h"

#include "game/game.h"
#include "game/window.h"

#include "util/logger.h"
#include "util/resources.h"

static void on_glfw_error_callback(int code, const char *message)
{
    logger_warn("GLFW Error: %s", message);
}

int main(void)
{
    if (!glfwInit())
        logger_fatal("GLFW initialization failed");

    glfwSetErrorCallback(on_glfw_error_callback);

    window_initialize();

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        window_free();
        glfwTerminate();
        logger_fatal("GLAD initialization failed");
    }

    glfwSwapInterval(1);

    resources_load();
    game_initialize();

    while (!glfwWindowShouldClose(window_get_instance()->handle)) {
        glClearColor(1.0f, 0.5f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        game_loop();

        glfwSwapBuffers(window_get_instance()->handle);
        glfwPollEvents();
    }

    game_free();
    resources_free();
    window_free();

    glfwTerminate();
    return 0;
}