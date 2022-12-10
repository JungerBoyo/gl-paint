#include <App.hpp>

#include <spdlog/spdlog.h>

using namespace glpaint;

App::App() : 
    window_layer_{event_queue_}, 
    gl_layer_{[](u32, u32, u32 id, u32, i32, const char *message, const void *) {
		spdlog::error("[OPENGL]{}:{}", id, message);
	}, 640, 480},
	ui_layer_{window_layer_.window.native(), event_queue_},
	app_event_polling_thread_([&app = *this]{ app(); }), 
    app_running_{true} 
{}

void App::operator()() {
    while (app_running_) {
        auto ev = event_queue_.waitingPop();
        if (ev.has_value()) {
        }
    }

    if (app_event_polling_thread_.joinable()) {
        app_event_polling_thread_.join();
    }
}

void App::run() {
    auto &window = window_layer_.window;

    while (!window.shouldClose()) {
        window.pollEvents();

        auto viewport_texture_id = gl_layer_.update();
        ui_layer_.update(viewport_texture_id);

        window.swapBuffers();
    }

    app_running_ = false;
}

void App::deinit() {
    gl_layer_.deinit();
    ui_layer_.deinit();
    window_layer_.deinit();
}