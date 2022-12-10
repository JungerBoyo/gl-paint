#include <ShadersRepository.hpp>

using namespace glpaint;

ShadersRepositoryRuntimeCompiled::ShadersRepositoryRuntimeCompiled() :
    workspace_viewport_shader_(Shader::Type::VERTEX_FRAGMENT,{
        "shaders/src/workspace_viewport_shader/shader.vert",
        "shaders/src/workspace_viewport_shader/shader.frag"
    }) {
}
const Shader& ShadersRepositoryRuntimeCompiled::workspaceViewportShader() const {
    return workspace_viewport_shader_;
}
void ShadersRepositoryRuntimeCompiled::deinit() {
    workspace_viewport_shader_.deinit();
}

ShadersRepositoryPrecompiled::ShadersRepositoryPrecompiled() :
    workspace_viewport_shader_(Shader::Type::VERTEX_FRAGMENT,{
        "shaders/bin/workspace_viewport_shader/vert.spv",
        "shaders/bin/workspace_viewport_shader/frag.spv"
    }) {
}
const Shader& ShadersRepositoryPrecompiled::workspaceViewportShader() const {
    return workspace_viewport_shader_;
}
void ShadersRepositoryPrecompiled::deinit() {
    workspace_viewport_shader_.deinit();
}