#include <Window.hpp>

#include <stdexcept>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

using namespace glpaint;

struct Window::WinNative {
	GLFWwindow *value{nullptr};
};

Window::Window(std::string_view title, i32 w, i32 h, void (*win_error_callback)(i32 err_code, const char* message))
	: win_handle_(std::make_shared<WinNative>()) {

	/// GLFW INIT
	if (glfwInit() != GLFW_TRUE) {
		throw std::runtime_error("glfw init failed");
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, SHCONFIG_GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, SHCONFIG_GL_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	win_handle_->value = glfwCreateWindow(w, h, title.data(), nullptr, nullptr);
	if (win_handle_->value == nullptr) {
		throw std::runtime_error("window creation failed");
	}
	glfwMakeContextCurrent(win_handle_->value);

	if (win_error_callback != nullptr) {
		glfwSetErrorCallback(win_error_callback);
	}

	glfwSetInputMode(win_handle_->value, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	/// GLAD INIT
}

void* Window::native() {
	return static_cast<void*>(win_handle_->value);
}
f32 Window::time() const {
	return static_cast<f32>(glfwGetTime());
}
std::pair<i32, i32> Window::size() const {
	i32 w{0};
	i32 h{0};
	glfwGetWindowSize(win_handle_->value, &w, &h);
	return {w, h};
}
void Window::setViewport(i32 w, i32 h) const {
	glViewport(0, 0, w, h);
}
void Window::setWinUserDataPointer(void* ptr){
	glfwSetWindowUserPointer(win_handle_->value, ptr);
}
template<>
void Window::setKeyCallback(void(*key_callback)(GLFWwindow*, i32, i32, i32, i32)) {
	glfwSetKeyCallback(win_handle_->value, key_callback);
}
template<>
void Window::setMousePositionCallback(void(*mouse_position_callback)(GLFWwindow*, f64, f64)) {
	glfwSetCursorPosCallback(win_handle_->value, mouse_position_callback);
}
template<>
void Window::setWindowResizeCallback(void(*window_resize_callback)(GLFWwindow*, i32, i32)) {
	glfwSetWindowSizeCallback(win_handle_->value, window_resize_callback);
}
template<>
void Window::setMouseScrollCallback(void(*mouse_scroll_callback)(GLFWwindow*, f64, f64)) {
	glfwSetScrollCallback(win_handle_->value, mouse_scroll_callback);
}

void Window::swapBuffers() const {
	glfwSwapBuffers(win_handle_->value);
}
bool Window::shouldClose() const {
	return glfwWindowShouldClose(win_handle_->value) != 0;
}
void Window::pollEvents() const {
	glfwPollEvents();
}

void Window::deinit() {
	glfwDestroyWindow(win_handle_->value);
	win_handle_.reset();
	glfwTerminate();
}