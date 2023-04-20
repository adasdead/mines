#include "glfw_callbacks.h"

#include "game/game.h"
#include "game/window.h"

#include "util/logger.h"

void on_glfw_error_callback(int code, const char *message)
{
    logger_warn("GLFW Error: %s", message);
}

void on_key_callback(GLFWwindow *win, int key, int scancode, int act, int mods)
{
    if (act != GLFW_PRESS) return;

    switch (key) {
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window_glfw(), GLFW_TRUE);
        break;
    
    case GLFW_KEY_D:
        game_toggle_difficulty();
        break;
    }
}

void on_mouse_click_callback(GLFWwindow *win, int btn, int act, int mods)
{
    int x, y;

    window_cursor_pos(&x, &y);

    switch (btn) {
    case GLFW_MOUSE_BUTTON_RIGHT:
        game_on_right_click(x, y, act == GLFW_PRESS);
        break;
    
    case GLFW_MOUSE_BUTTON_LEFT:
        game_on_left_click(x, y, act == GLFW_PRESS);
        break;
    }
}