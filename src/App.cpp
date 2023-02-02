#include <App.hpp>

#include <spdlog/spdlog.h>

using namespace glpaint;

App::App() : 
    window_layer_{event_queue_}, 
    gl_layer_{[](u32, u32, u32 id, u32, i32, const char *message, const void *) {
		spdlog::error("[OPENGL]{}:{}", id, message);
	}, SHCONFIG_INITIAL_WINDOW_WIDTH, SHCONFIG_INITIAL_WINDOW_HEIGHT},
	ui_layer_{window_layer_.window.native(), event_queue_},
	app_event_polling_thread_([
        &app = *this, 
        stop_token = app_stop_source_.get_token()
    ]{ app(stop_token); }) 
{}

void App::operator()(std::stop_token stop_token) {
    std::stop_callback stop_callback(
        stop_token,
        [&event_queue=event_queue_] {
            event_queue.emplace(std::make_unique<CancellationEvent>());
        }
    );

    while (true) {
        const auto ev = event_queue_.waitingPop();
        if (!ev.has_value()) { continue; }

        if ((ev.value()->tag() & Event::Tag::APPLICATION) > 0) {
            if (ev.value()->type() == Event::Type::CANCELLATION) { break; }
            switch (ev.value()->type()) {
            default: break;
            }
        }

        if ((ev.value()->tag() & Event::Tag::GL_LAYER) > 0) {
            switch(ev.value()->type()) { // NOLINT
            case Event::Type::WINDOW_RESIZE: {
                const auto* win_resize_ev = Event::dispatch<WindowResizeEvent>(ev.value().get());
                gl_layer_.emplaceSynchronousCommand([
                    &gl_layer = gl_layer_,
                    width = win_resize_ev->width,
                    height = win_resize_ev->height]{
                    gl_layer.updateViewportCommand(width, height);
                });
                break;
            }
            case Event::Type::IMAGE_LAYER_SWAP: {
                const auto* img_layer_swap_ev = Event::dispatch<ImageLayerSwapEvent>(ev.value().get());
                gl_layer_.emplaceSynchronousCommand([
                    &gl_layer = gl_layer_,
                    src_layer_index = img_layer_swap_ev->src_layer_index,
                    dst_layer_index = img_layer_swap_ev->dst_layer_index]{
                    gl_layer.updateImageLayerSwapCommand(
                        src_layer_index,
                        dst_layer_index
                    );
                });

                break;
            }
            case Event::Type::LAYER_ADD: {
                // const auto* add_layer_ev = Event::dispatch<LayerAddEvent>(ev.value().get());
                gl_layer_.emplaceSynchronousCommand([&gl_layer = gl_layer_]{
                    gl_layer.updateAddLayerCommand();
                });
            }
            case Event::Type::SCROLL: {
                const auto* scroll_ev = Event::dispatch<ScrollEvent>(ev.value().get());
                gl_layer_.emplaceSynchronousCommand([
                    &gl_layer = gl_layer_,
                    zoom_value = static_cast<f32>(scroll_ev->y)]{
                    gl_layer.updateZoomWorkspaceCommand(zoom_value);
                });
            }
            }
        }
    }
}

void App::run() {
    auto &window = window_layer_.window;

    while (!window.shouldClose()) {
        window.pollEvents();

        gl_layer_.clearColorBuffer();

        const auto[win_width, win_height] = window.size();
        
        const auto gl_layer_update_info = gl_layer_.update(win_width, win_height);

        ui_layer_.update(
            gl_layer_update_info.viewport_tex_id, 
            gl_layer_update_info.viewport_width, 
            gl_layer_update_info.viewport_height, 
            gl_layer_update_info.layer_view_ids
        );

        window.swapBuffers();
    }

    app_stop_source_.request_stop();
    if (app_event_polling_thread_.joinable()) {
        app_event_polling_thread_.join();
    }
}

void App::deinit() {
    gl_layer_.deinit();
    ui_layer_.deinit();
    window_layer_.deinit();
}