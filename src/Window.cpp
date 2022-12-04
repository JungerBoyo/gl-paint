#include "Window.hpp"

#include <stdexcept>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

using namespace template_project;

struct Window::WinNative {
	GLFWwindow *value{nullptr};
};

Window::Window(std::string_view title, int w, int h,
			   void (*win_error_callback)(int, const char *),
			   void (*gl_error_callback)(std::uint32_t, std::uint32_t,
										 std::uint32_t, std::uint32_t, int,
										 const char *, const void *))
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
	if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == // NOLINT
		0) {
		throw std::runtime_error("glad loader failed");
	}
	if (gl_error_callback != nullptr) {
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
							  GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr,
							  GL_TRUE);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
							  GL_DEBUG_SEVERITY_LOW, 0, nullptr,
							  GL_TRUE);
		glDebugMessageCallback(gl_error_callback, nullptr);
	}
	//glClearColor(0.22F, 0.61F, 0.78F, 1.F);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void* Window::native() {
	return static_cast<void*>(win_handle_->value);
}
float Window::time() const {
	return static_cast<float>(glfwGetTime());
}
std::pair<int, int> Window::size() const {
	int w{0};
	int h{0};
	glfwGetWindowSize(win_handle_->value, &w, &h);
	return {w, h};
}
void Window::setViewport(int w, int h) const {
	glViewport(0, 0, w, h);
}
void Window::setWinUserDataPointer(void* ptr){
	glfwSetWindowUserPointer(win_handle_->value, ptr);
}
template<>
void Window::setKeyCallback<void, GLFWwindow*, int, int, int, int>
(void(*key_callback)(GLFWwindow*, int, int, int, int)) {
	glfwSetKeyCallback(win_handle_->value, key_callback);
}
template<>
void Window::setMousePositionCallback<void, GLFWwindow*, double, double>
(void(*mouse_position_callback)(GLFWwindow*, double, double)) {
	glfwSetCursorPosCallback(win_handle_->value, mouse_position_callback);
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