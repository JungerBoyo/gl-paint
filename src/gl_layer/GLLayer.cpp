#include <GLLayer.hpp>

#include <stdexcept>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

using namespace glpaint;

static bool isSpirvSupported();

std::unique_ptr<ShadersRepository> GLLayer::shaders_repo_{ nullptr };

GLLayer::Context::Context() {
    if ((result = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) == 0) { // NOLINT
        throw std::runtime_error("glad loader failed");
    }
}

GLLayer::GLLayer(GLErrorCallback gl_error_callback, i32 viewport_w, i32 viewport_h) :
    main_fbo_({{0, GL_RGBA8}}, viewport_w, viewport_h),
    workspace_(viewport_w, viewport_h) {
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
        glDebugMessageControl(
            GL_DONT_CARE, GL_DONT_CARE, 
            GL_DEBUG_SEVERITY_MEDIUM, 0, nullptr, 
            GL_TRUE
        );
        glDebugMessageCallback(gl_error_callback, nullptr);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (isSpirvSupported()) {
        shaders_repo_ = std::make_unique<ShadersRepositoryPrecompiled>();
    } else {
        shaders_repo_ = std::make_unique<ShadersRepositoryRuntimeCompiled>();
    }
}

GLLayer::UpdateInfo GLLayer::update(i32 window_width, i32 window_height) {
    if (const auto command = synchronous_command_queue_.pop(); command.has_value()) {
        command.value()();
    }

    main_fbo_.bind();
    const auto[fbo_width, fbo_height] = main_fbo_.size();
    glViewport(0, 0, fbo_width, fbo_height);

    quad_.bind();
    clearColorBuffer();

    workspace_.update();

    main_fbo_.unbind();

    glViewport(0, 0, window_width, window_height);

    return {
        main_fbo_.textureId(0), 
        fbo_width, 
        fbo_height,
        workspace_.layerViews()
    };
}

void GLLayer::updateViewportCommand(i32 new_width, i32 new_height) {
    // TODO
}
void GLLayer::updateImageLayerSwapCommand(i32 src_layer_index, i32 dst_layer_index) {
    workspace_.swapLayerTextures(src_layer_index, dst_layer_index);
}
void GLLayer::updateAddLayerCommand() {
    workspace_.addLayerTexture();
}
void GLLayer::updateZoomWorkspaceCommand(f32 zoom_value) {
    workspace_.addScale(zoom_value);
}

void GLLayer::clearColorBuffer() const {
    glClear(GL_COLOR_BUFFER_BIT);
}

void GLLayer::deinit() {
    quad_.deinit();
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