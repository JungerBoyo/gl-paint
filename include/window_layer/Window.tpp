struct GLFWwindow;

template<>
void glpaint::Window::setKeyCallback
(void(*key_callback)(GLFWwindow*, i32, i32, i32, i32));

template<>
void glpaint::Window::setMousePositionCallback
(void(*mouse_position_callback)(GLFWwindow*, f64, f64));

template<>
void glpaint::Window::setWindowResizeCallback
(void(*window_resize_callback)(GLFWwindow*, i32, i32));

template<>
void glpaint::Window::setMouseScrollCallback
(void(*mouse_scroll_callback)(GLFWwindow*, f64, f64));