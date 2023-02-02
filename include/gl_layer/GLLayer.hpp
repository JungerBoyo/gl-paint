#ifndef GLPAINT_GL_LAYER_HPP
#define GLPAINT_GL_LAYER_HPP

#include <Framebuffer.hpp>
#include <ShadersRepository.hpp>
#include <Quad.hpp>
#include <Workspace.hpp>
#include <RingQueue.hpp>
#include <common.hpp>

#include <memory>
#include <functional>
#include <span>

namespace glpaint {

using CommandQueue = RingQueue<std::function<void()>, 32>;

struct GLLayer {
	using GLErrorCallback = void (*)(
        u32         source,
        u32         type,
        u32         id,
        u32         severity,
        i32         length,
        const char* message,
        const void* user_param
    );
    /**
     * @brief structure returned on every update
     */
    struct UpdateInfo { 
        u32 viewport_tex_id; 
        i32 viewport_width;
        i32 viewport_height;
        std::span<const u32> layer_view_ids; // views_into_layers
    };

private:
    struct Context {
        Context();
        i32 result{ 1 };
    };
    /**
     * @brief dummy context to assure glad initialization before anything else
     */
    Context context_;
    /**
     * @brief gl layer holds repo of shaders so that shaders
     * are loaded/compiled once and used across application
     */
    static std::unique_ptr<ShadersRepository> shaders_repo_;
    /**
     * @brief gl layer renders to its own framebuffer, which can
     * be reused later on
     */
    Framebuffer main_fbo_;
    /**
     * @brief quad, every render call uses quad which is bound once in
     * main update method
     */
    Quad quad_;
    /**
     * @brief workspace, consists of image layer rendered in specified 
     * order 
     */
    Workspace workspace_;
    /**
     * @brief 
     * 
     */
    CommandQueue synchronous_command_queue_;
public:
	GLLayer(GLErrorCallback gl_error_callback, i32 viewport_w, i32 viewport_h);

    /**
     * @brief update gl layer and returns tex id + v width + v height
     * 
     * @param window_width current window width
     * @param window_height current window height 
     * @return UpdateInfo 
     */
	[[nodiscard]] UpdateInfo update(i32 window_width, i32 window_height); 

    void emplaceSynchronousCommand(std::function<void()>&& command) {
        synchronous_command_queue_.emplace(std::move(command));
    }

    // COMMANDS
    void updateViewportCommand(i32 new_width, i32 new_height);
    void updateImageLayerSwapCommand(i32 src_layer_index, i32 dst_layer_index);
    void updateAddLayerCommand();
    void updateZoomWorkspaceCommand(f32 zoom_value);

    void clearColorBuffer() const;

    [[nodiscard]] static const std::unique_ptr<ShadersRepository>&
    shaderRepository() { return shaders_repo_; }

	void deinit();
};

}

#endif