#include <WindowLayer.hpp>
#include <Event.hpp>

#include <spdlog/spdlog.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string_view>
#include "config.hpp"


using namespace glpaint;

#define EVENT_QUEUE_ACCESS auto* event_queue = static_cast<EventQueue*>(glfwGetWindowUserPointer(win_native))

WindowLayer::WindowLayer(EventQueue& event_queue) :
    event_queue_(event_queue),
    window{
        cmake::project_name,
        SHCONFIG_INITIAL_WINDOW_WIDTH, SHCONFIG_INITIAL_WINDOW_HEIGHT,
        [](i32 err_code, const char* message){
            spdlog::error("[GLFW3]{}:{}", err_code, message);
        }    
    }{

    window.setWinUserDataPointer(&event_queue_);

    window.setWindowResizeCallback(
    +[](GLFWwindow* win_native, i32 width, i32 height) {
        EVENT_QUEUE_ACCESS;
        event_queue->emplace(std::make_unique<WindowResizeEvent>(width, height));
    });

    window.setMouseScrollCallback(
    +[](GLFWwindow* win_native, f64 x, f64 y) {
        EVENT_QUEUE_ACCESS;
        event_queue->emplace(std::make_unique<ScrollEvent>(x, y));
    });
}

void WindowLayer::deinit() {
    window.deinit();
}
