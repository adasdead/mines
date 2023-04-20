#ifndef GLFW_CALLBACKS_H
#define GLFW_CALLBACKS_H

#include "graphics/opengl.h"

void on_glfw_error_callback(int code, const char *message);
void on_key_callback(GLFWwindow *win, int key, int scancode, int act, int mods);
void on_mouse_click_callback(GLFWwindow *win, int btn, int act, int mods);

#endif /* GLFW_CALLBACKS_H */