#include "window.h"

#include "game/difficulty.h"

#include "util/logger.h"

#include "definitions.h"

static mat4 projection = NULL;
static GLFWwindow *window;
static double factor = 0.0;

void window_init(void)
{
    uint width = FIELD_LX + difficulties->field_width + FIELD_RX;
    uint height = FIELD_LY + difficulties->field_height + FIELD_RY;
    uint win_width = width * window_scale_factor();
    uint win_height = height * window_scale_factor();

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(win_width, win_height, WINDOW_NAME, NULL, NULL);

    if (window == NULL) {
        glfwTerminate();
        logger_fatal("Window creation error");
    }

    glfwMakeContextCurrent(window);

    logger_info("Window created(%dx%d)", win_width, win_height);
}

void window_resize(uint width, uint height)
{
    uint win_width = width * window_scale_factor();
    uint win_height = height * window_scale_factor();

    glfwSetWindowSize(window_glfw(), win_width, win_height);
    glViewport(0, 0, win_width, win_height);

    if (projection != NULL) matrix4x4_free(projection);
    
    projection = matrix4x4_ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f);

    logger_info("Window resized(%dx%d)", win_width, win_height);
}

double window_scale_factor(void)
{
    if (factor == 0.0) {
        const GLFWvidmode *video_mode;
        video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        factor = (video_mode->height) * 1.f / WINDOW_SCALE;
        logger_info("Scale factor: %f", factor);
    }

    return factor;
}

void window_normalize_cursor_pos(int *x, int *y)
{
    *x = *x / window_scale_factor();
    *y = *y / window_scale_factor();
}

void window_cursor_pos(int *x, int *y)
{
    double dx, dy;

    glfwGetCursorPos(window_glfw(), &dx, &dy);

    *x = (int) dx;
    *y = (int) dy;
}

void window_free(void)
{
    glfwDestroyWindow(window_glfw());
    matrix4x4_free(projection);
}

GLFWwindow *window_glfw(void)
{
    return window;
}

mat4 window_projection(void)
{
    return projection;
}