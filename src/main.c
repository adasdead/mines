#include "graphics/opengl.h"

#include "game/game.h"
#include "game/window.h"

#include "util/logger.h"
#include "util/resources.h"

static int init(void);
static int loop(void);
static int free_all(void);

int main(void)
{
    return init();
}

static void on_glfw_error_callback(int code, const char *message)
{
    logger_warn("GLFW Error: %s", message);
}

static int init(void)
{
    if (!glfwInit()) {
        logger_fatal("GLFW initialization failed");
    }

    glfwSetErrorCallback(on_glfw_error_callback);

    window_init();

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        window_free();
        glfwTerminate();
        logger_fatal("GLAD initialization failed");
    }

    glfwSwapInterval(1);

    resources_load();
    game_init();

    return loop();
}

static int loop(void)
{
    while (!glfwWindowShouldClose(window_instance()->handle)) {
        glClearColor(1.0f, 0.5f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        game_loop();

        glfwSwapBuffers(window_instance()->handle);
        glfwPollEvents();
    }

    return free_all();
}

static int free_all(void)
{
    game_free();
    resources_free();
    window_free();

    glfwTerminate();

    return 0;
}