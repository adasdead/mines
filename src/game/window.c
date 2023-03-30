#include "window.h"

#include "util/logger.h"

#include "definitions.h"

static GLFWwindow *window;
static m4x4 projection;
static double factor;

static double window_scale_factor(void)
{
    const GLFWvidmode *video_mode;
    video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    return (video_mode->height) * 1.f / SCREEN_BASE_HEIGHT;
}

void window_init(void)
{
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WINDOW_BASE_WIDTH, WINDOW_BASE_HEIGHT,
                              WINDOW_NAME, NULL, NULL);

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
    factor = window_scale_factor();
    u32 win_width = CELL_WIDTH * factor * width;
    u32 win_height = CELL_WIDTH * factor * height;

    glfwSetWindowSize(window_glfw(), win_width, win_height);
    glViewport(0, 0, win_width, win_height);

    matrix4x4_free(projection);
    
    projection = projection_ortho(0.0f, width, height,
                                  0.0f, -1.0f, 1.0f);
}

void window_mouse_pos(i32 *x, i32 *y)
{
    double xpos, ypos;
    
    glfwGetCursorPos(window_glfw(), &xpos, &ypos);

    *x = (i32) xpos / CELL_WIDTH / factor;
    *y = (i32) ypos / CELL_WIDTH / factor;
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

m4x4 window_projection(void)
{
    return projection;
}