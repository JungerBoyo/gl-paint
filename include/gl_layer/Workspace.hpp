#ifndef GLPAINT_WORKSPACE_HPP
#define GLPAINT_WORKSPACE_HPP

#include <common.hpp>
#include <Texture2DArray.hpp>
#include <Shader.hpp>

#include <vector>

namespace glpaint {

struct Workspace {
    struct Transform {
        f32 offset[2] = {0.F, 0.F};
        f32 scale[2] = {1.F, 1.F};
    };
    struct InstanceToTexture {
        u32 texture_index;
    };

private:
    /**
     * @brief initial count of allocated image layers in workspace
     */
    static constexpr i32 INITIAL_CAPACITY{ 4 };
    /**
     * @brief initial size base + workspace image
     */
    static constexpr i32 INITIAL_SIZE{ 2 };
    /**
     * @brief index of base image(first image)
     */
    static constexpr i32 BASE_TEX2D_INDEX{ 0 };
    /**
     * @brief index of implicit image(image that's always above 
     * currently selected layer and holds intermediate result
     * of last operation performed on selected image layer)
     */
    static constexpr i32 SANDBOX_TEX2D_INDEX{ 1 };
    /**
     * @brief workspace image array which is a handle to image layers data 
     * on GPU
     */
    Texture2DArray workspace_tex2d_array_;
    /**
     * @brief extent of workspace, each image layer will be this width and 
     * height
     */
    i32 workspace_w_;
    i32 workspace_h_;
    /**
     * @brief global workspace transform
     */
    Transform workspace_transform_{};
    /**
     * @brief layers are drawn using instancing
     */
    std::vector<i32> instance_to_texture_translations_;
public:
    Workspace(i32 workspace_w, i32 workspace_h);

    void update();

    void deinit();
};

}

#endif