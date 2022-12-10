#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

template<>
void glpaint::Window::setKeyCallback
(void(*key_callback)(GLFWwindow*, i32, i32, i32, i32));

template<>
void glpaint::Window::setMousePositionCallback
(void(*mouse_position_callback)(GLFWwindow*, f64, f64));