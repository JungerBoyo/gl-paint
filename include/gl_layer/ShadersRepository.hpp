#ifndef GLPAINT_SHADERS_REPOSITORY_HPP
#define GLPAINT_SHADERS_REPOSITORY_HPP

#include <Shader.hpp>

#include <vector>

namespace glpaint {

struct ShadersRepository {
    virtual ~ShadersRepository() = default;
    
    virtual void deinit() = 0;
    virtual const Shader& workspaceViewportShader() const = 0;
};

struct ShadersRepositoryRuntimeCompiled : public ShadersRepository {
private:
    Shader workspace_viewport_shader_;

public:
    ShadersRepositoryRuntimeCompiled();

    void deinit() override;
    const Shader& workspaceViewportShader() const override;
};
struct ShadersRepositoryPrecompiled : public ShadersRepository {
private:
    Shader workspace_viewport_shader_;

public:
    ShadersRepositoryPrecompiled();

    void deinit() override;
    const Shader& workspaceViewportShader() const override;
};

}

#endif