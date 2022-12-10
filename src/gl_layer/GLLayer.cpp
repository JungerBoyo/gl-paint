#include <GLLayer.hpp>

#include <stdexcept>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

using namespace glpaint;

static bool isSpirvSupported();

std::unique_ptr<ShadersRepository> GLLayer::shaders_repo_{ nullptr };

GLLayer::GLLayer(GLErrorCallback gl_error_callback, i32 viewport_w, i32 viewport_h) :
    main_fbo_({{1, GL_RGBA32F}}, viewport_w, viewport_h) {
    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0) { // NOLINT
        throw std::runtime_error("glad loader failed");
    }
    if (gl_error_callback != nullptr) {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageControl(
            GL_DONT_CARE, GL_DONT_CARE,
            GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr,
            GL_TRUE
        );
        glDebugMessageControl(
            GL_DONT_CARE, GL_DONT_CARE, 
            GL_DEBUG_SEVERITY_LOW, 0, nullptr, 
            GL_TRUE
        );
        glDebugMessageCallback(gl_error_callback, nullptr);
    }

    shaders_repo_ = isSpirvSupported() ? 
        std::unique_ptr<ShadersRepository>(new ShadersRepositoryPrecompiled()) :
        std::unique_ptr<ShadersRepository>(new ShadersRepositoryRuntimeCompiled());
}

u32 GLLayer::update() {
    main_fbo_.bind();

    // render stuff here

    main_fbo_.unbind();
    return main_fbo_.textureId(0);
}

void GLLayer::deinit() {
    shaders_repo_->deinit();
}

bool isSpirvSupported() {
    int ext_num{0};
    bool is_arb_spirv_supported{false};
    glGetIntegerv(GL_NUM_EXTENSIONS, &ext_num);
    for (int i{0}; i<ext_num; ++i) {
        const auto ext_str = 
            std::string_view(
                reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i)) // NOLINT
            );
        if (ext_str == "GL_ARB_gl_spirv") {
            is_arb_spirv_supported = true;
            break;
        }
    }

    return is_arb_spirv_supported;
}