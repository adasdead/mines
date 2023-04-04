#include "window.h"

#include "util/logger.h"

#include "definitions.h"

static GLFWwindow *window;
static mat4 projection;
static double factor = 0.0;

static double window_scale_factor(void)
{
    if (factor == 0.0) {
        const GLFWvidmode *video_mode;
        video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        factor = (video_mode->height) * 1.f / SCREEN_BASE_HEIGHT;
    }

    return factor;
}

void window_init(void)
{
    u32 win_width = CELL_WIDTH_PX * 11 * window_scale_factor();
    u32 win_height = CELL_WIDTH_PX * 14 * window_scale_factor();

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(win_width, win_height, WINDOW_NAME,
                              NULL, NULL);

    if (window == NULL) {
        glfwTerminate();
        logger_fatal("Window creation error");
    }

    glfwMakeContextCurrent(window);

    projection = matrix4x4_allocate(false);

    logger_info("Window created(%dx%d)", WINDOW_BASE_WIDTH,
                WINDOW_BASE_HEIGHT);
}

void window_resize(u32 width, u32 height)
{
    u32 win_width = CELL_WIDTH_PX * window_scale_factor() * width;
    u32 win_height = CELL_WIDTH_PX * window_scale_factor() * height;

    glfwSetWindowSize(window_glfw(), win_width, win_height);
    glViewport(0, 0, win_width, win_height);

    matrix4x4_free(projection);
    
    projection = projection_ortho(0.0f, width, height,
                                  0.0f, -1.0f, 1.0f);
}

void window_normalize_mouse_pos(i32 *x, i32 *y)
{
    *x = *x / CELL_WIDTH_PX / window_scale_factor();
    *y = *y / CELL_WIDTH_PX / window_scale_factor();
}

void window_free(void)
{
    glfwDestroyWindow(window_glfw());
    matrix4x4_free(projection);
}

double window_factor(void)
{
    return factor;
}

GLFWwindow *window_glfw(void)
{
    return window;
}

mat4 window_projection(void)
{
    return projection;
}