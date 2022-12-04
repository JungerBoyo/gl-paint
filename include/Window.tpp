#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

template<>
void template_project::Window::setKeyCallback
(void(*key_callback)(GLFWwindow*, int, int, int, int));

template<>
void template_project::Window::setMousePositionCallback
(void(*mouse_position_callback)(GLFWwindow*, double, double));