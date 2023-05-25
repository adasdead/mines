/* 
 * Copyright (c) 2023 adasdead
 * This software is licensed under the MIT License.
 */

#include "window.h"

#include <stb/stb_image.h>

#include "game/difficulty.h"
#include "game/game.h"

#include "util/logger.h"

#include "definitions.h"

static struct window window = {0};

static void cursor_pos_callback(GLFWwindow *win, double x, double y)
{
    window.cursor.x = (int) x;
    window.cursor.y = (int) y;
}

static void
key_callback(GLFWwindow *win, int key, int code, int act, int mods)
{
    if (act != GLFW_PRESS) return;

    switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window.handle, GLFW_TRUE);
            break;
        
        case GLFW_KEY_D:
            game_toggle_difficulty();
            break;

        case GLFW_KEY_N:
            game_reset();
            break;
    }
}

static void
mouse_button_callback(GLFWwindow *win, int btn, int act, int mods)
{
    bool pressed = (act == GLFW_PRESS);

    switch (btn) {
        case GLFW_MOUSE_BUTTON_RIGHT:
            game_on_right_click(window.cursor.x, window.cursor.y,
                                pressed);
            break;
        
        case GLFW_MOUSE_BUTTON_LEFT:
            game_on_left_click(window.cursor.x, window.cursor.y,
                               pressed);
            break;
    }
}

void window_initialize(void)
{
    GLFWimage icon_image;
    const GLFWvidmode *video_mode;

    if (window.initialized) {
        logger_warn("Window already initialized!");
        return;
    }

    video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    window.scale = (video_mode->height) * (1.f / WINDOW_SCALE);
    logger_info("Scale factor: %f", window.scale);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window.width = FIELD_LEFT_X + FIELD_RIGHT_X;
    window.width += difficulties->field_width;
    window.width = (uint) DENORMALIZE(window.width);
    
    window.height = FIELD_LEFT_Y + FIELD_RIGHT_Y;
    window.height += difficulties->field_height;
    window.height = (uint) DENORMALIZE(window.height);

    window.handle = glfwCreateWindow(window.width, window.height,
                                     WINDOW_NAME, NULL, NULL);

    if (window.handle == NULL) {
        glfwTerminate();
        logger_fatal("Window creation error");
    }

    glfwSetKeyCallback(window.handle, key_callback);
    glfwSetCursorPosCallback(window.handle, cursor_pos_callback);
    glfwSetMouseButtonCallback(window.handle, mouse_button_callback);

    icon_image.pixels = stbi_load(ASSETS_DIR"/icon.png",
                                  &icon_image.width,
                                  &icon_image.height,
                                  NULL, STBI_rgb_alpha);

    glfwSetWindowIcon(window.handle, 1, &icon_image);

    stbi_image_free(icon_image.pixels);

    glfwMakeContextCurrent(window.handle);

    logger_info("Window created(%dx%d)", window.width, window.height);

    window.initialized = true;
}

void window_resize_with_normalized_sizes(int width, int height)
{
    window.width = (uint) DENORMALIZE(width);
    window.height = (uint) DENORMALIZE(height);

    glViewport(0, 0, window.width, window.height);

    glfwSetWindowSize(window.handle, window.width, window.height);

    matrix4x4_free(window.projection);

    window.projection = matrix4x4_ortho(0.0f,
                                        (float) width,
                                        (float) height,
                                        0.0f, -1.0f, 1.0f);

    logger_info("Window resized(%dx%d)", window.width, window.height);
}

void window_normalize_pos(int *x, int *y)
{
    *x = ((int) (*x / window.scale));
    *y = ((int) (*y / window.scale));
}

void window_free(void)
{
    if (window.initialized) {
        glfwDestroyWindow(window.handle);
        matrix4x4_free(window.projection);
    }
}

const window_t window_get_instance(void)
{
    return &window;
}