#include "graphics/opengl.h"

#include "game/game.h"
#include "game/window.h"

#include "util/logger.h"
#include "util/resources.h"

#include "glfw_callbacks.h"

static int init(void);
static int loop(void);
static int free_all(void);

int main(void)
{
    return init();
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

    glfwSetKeyCallback(window_glfw(), on_key_callback);
    glfwSetMouseButtonCallback(window_glfw(), on_mouse_click_callback);

    glfwSwapInterval(1);

    resources_load();
    game_init();

    return loop();
}

static int loop(void)
{
    while (!glfwWindowShouldClose(window_glfw())) {
        glfwPollEvents();
        
        glClearColor(1.0f, 0.5f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        game_loop();

        glfwSwapBuffers(window_glfw());
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