#ifndef GLPAINT_GL_LAYER_HPP
#define GLPAINT_GL_LAYER_HPP

#include <Framebuffer.hpp>
#include <ShadersRepository.hpp>
#include <common.hpp>

#include <memory>

namespace glpaint {

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

private:
    static std::unique_ptr<ShadersRepository> shaders_repo_;
    Framebuffer main_fbo_;
    
public:
	GLLayer(GLErrorCallback gl_error_callback, i32 viewport_w, i32 viewport_h);

	[[nodiscard]] u32 update(); 

    [[nodiscard]] static const std::unique_ptr<ShadersRepository>&
    shaderRepository() { return shaders_repo_; }

	void deinit();
};

}

#endif