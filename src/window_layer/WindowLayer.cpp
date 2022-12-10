#include <WindowLayer.hpp>

#include <spdlog/spdlog.h>

#include <string_view>
#include "config.hpp"

using namespace glpaint;

WindowLayer::WindowLayer(EventQueue& event_queue) :
    event_queue_(event_queue),
    window{
        cmake::project_name,
        640, 480,
        [](i32 err_code, const char* message){
            spdlog::error("[GLFW3]{}:{}", err_code, message);
        }    
    }
{}

void WindowLayer::deinit() {
    window.deinit();
}
