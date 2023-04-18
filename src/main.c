#include "graphics/opengl.h"

#include "game/game.h"
#include "game/window.h"

#include "util/logger.h"
#include "util/resources.h"

static void on_glfw_error_callback(int code, const char *message)
{
    logger_warn("GLFW Error: %s", message);
}

static void on_key_callback(GLFWwindow* window, int key, int scancode,
                            int action, int mods)
{
    if (action != GLFW_PRESS) return;

    switch (key) {
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window_glfw(), GLFW_TRUE);
        break;
    
    case GLFW_KEY_D:
        game_toggle_difficulty();
        break;
    }
}

void on_mouse_click_callback(GLFWwindow* window, int button,
                             int action, int mods)
{
    int x, y;

    window_cursor_pos(&x, &y);

    switch (button) {
    case GLFW_MOUSE_BUTTON_RIGHT:
        game_on_right_click(x, y, action == GLFW_PRESS);
        break;
    
    case GLFW_MOUSE_BUTTON_LEFT:
        game_on_left_click(x, y, action == GLFW_PRESS);
        break;
    }
}

static void init(void)
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
    glfwSetMouseButtonCallback(window_glfw(),
                               on_mouse_click_callback);

    glfwSwapInterval(1);
}

int main(void)
{
    init();
    resources_load();
    game_init();

    while (!glfwWindowShouldClose(window_glfw())) {
        glfwPollEvents();
        
        glClearColor(1.0f, 0.5f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        game_loop();

        glfwSwapBuffers(window_glfw());
    }

    game_free();
    resources_free();
    window_free();

    glfwTerminate();
    return 0;
}