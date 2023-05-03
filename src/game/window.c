#include "window.h"

#include "game/difficulty.h"
#include "game/game.h"

#include "util/logger.h"

#include "definitions.h"

static struct window window = {0};

static void window_cursor_pos_callback(GLFWwindow *win, double x, double y)
{
    window_t window = glfwGetWindowUserPointer(win);

    window->cursor.x = (int) x;
    window->cursor.y = (int) y;
}

void window_key_callback(GLFWwindow *win, int key, int code, int act, int mods)
{
    window_t window = glfwGetWindowUserPointer(win);

    if (act != GLFW_PRESS) return;

    switch (key) {
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window->handle, GLFW_TRUE);
        break;
    
    case GLFW_KEY_D:
        game_toggle_difficulty();
        break;
    }
}

void window_mouse_button_callback(GLFWwindow *win, int btn, int act, int mods)
{
    bool pressed = (act == GLFW_PRESS);
    window_t window = glfwGetWindowUserPointer(win);

    switch (btn) {
    case GLFW_MOUSE_BUTTON_RIGHT:
        game_on_right_click(window->cursor.x, window->cursor.y, pressed);
        break;
    
    case GLFW_MOUSE_BUTTON_LEFT:
        game_on_left_click(window->cursor.x, window->cursor.y, pressed);
        break;
    }
}

void window_init(void)
{
    const GLFWvidmode *video_mode;

    if (window.initialized) {
        logger_warn("Window already initialized!");
        return;
    }

    window.width = FIELD_LX + difficulties->field_width + FIELD_RX;
    window.height = FIELD_LY + difficulties->field_height + FIELD_RY;

    video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    window.scale = (video_mode->height) * 1.f / WINDOW_SCALE;
    logger_info("Scale factor: %f", window.scale);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window.width *= window.scale;
    window.height *= window.scale;

    window.handle = glfwCreateWindow(window.width, window.height, WINDOW_NAME,
                                     NULL, NULL);

    if (window.handle == NULL) {
        glfwTerminate();
        logger_fatal("Window creation error");
    }

    glfwSetWindowUserPointer(window.handle, &window);

    glfwSetKeyCallback(window.handle, window_key_callback);
    glfwSetCursorPosCallback(window.handle, window_cursor_pos_callback);
    glfwSetMouseButtonCallback(window.handle, window_mouse_button_callback);

    glfwMakeContextCurrent(window.handle);

    logger_info("Window created(%dx%d)", window.width, window.height);

    window.initialized = true;
}

void window_normalized_resize(int width, int height)
{
    window.width = width * window.scale;
    window.height = height * window.scale;

    glViewport(0, 0, window.width, window.height);

    glfwSetWindowSize(window.handle, window.width, window.height);

    matrix4x4_free(window.projection);

    window.projection = matrix4x4_ortho(0.0f, width, height,
                                        0.0f, -1.0f, 1.0f);

    logger_info("Window resized(%dx%d)", window.width, window.height);
}

void window_free(void)
{
    if (window.initialized) {
        glfwDestroyWindow(window.handle);
        matrix4x4_free(window.projection);
    }
}

window_t window_instance(void)
{
    return &window;
}