#ifndef GLPAINT_WORKSPACE_HPP
#define GLPAINT_WORKSPACE_HPP

#include <common.hpp>
#include <Texture2DArray.hpp>
#include <StorageBufferObject.hpp>
#include <Shader.hpp>
#include <Sampler.hpp>

#include <vector>
#include <span>

namespace glpaint {

struct GLLayer;

struct Workspace {
    struct Transform {
        f32 offset[2]{0.F, 0.F};
        f32 aspect_ratio{ 0.F };
        f32 scale{ 1.F };
    };

    struct alignas(16) InstanceToTextureTranslation {
        u32 texture_layer_index{ 0 };
        f32 alpha{ 1.F };
    };

private:
    /**
     * @brief initial count of allocated image layers in workspace
     * 1 sandbox layer + 3 user layers
     */
    static constexpr i32 INITIAL_CAPACITY{ 4 };
    /**
     * @brief max number of layers
     */
    static constexpr i32 MAX_CAPACITY{ 128 };
    /**
     * @brief index of base image(first image)
     */
    static constexpr i32 BASE_TEX2D_INDEX{ 1 };
    /**
     * @brief index of implicit image(image that's always above 
     * currently selected layer and holds intermediate result
     * of last operation performed on selected image layer)
     */
    static constexpr i32 SANDBOX_TEX2D_INDEX{ 0 };
    /**
     * @brief workspace image array which is a handle to image layers data 
     * on GPU
     */
    Texture2DArray workspace_tex2d_array_;
    /**
     * @brief defines sampling for basic image layer
     */
    Sampler basic_tex2d_sampler_;
    /**
     * @brief workspace ubo stores workspace_transform on the GPU
     */
    StorageBufferObject workspace_ubo_;
    /**
     * @brief extent of workspace, each image layer will be this width and 
     * height
     */
    i32 workspace_w_;
    i32 workspace_h_;
    /**
     * @brief global workspace transform
     */
    Transform workspace_transform_{
        .aspect_ratio = static_cast<f32>(workspace_w_) / static_cast<f32>(workspace_h_)
    };
    /**
     * @brief layers are drawn using instancing, they maybe in different
     * order yet still drawn using instancing(which is sequential)
     */
    std::vector<InstanceToTextureTranslation> instance_to_texture_translations_;
    /**
     * @brief same as above but this keeps track of texture views
     * to avoid memory allocation on every GET
     */
    std::vector<u32> instance_to_view_translations_;
    /**
     * @brief specifies number of active layers - sandbox layer
     */
    i32 num_active_layers{ 1 };
public:
    Workspace(i32 workspace_w, i32 workspace_h);

    void update();

    [[nodiscard]] auto layerViews() const { 
        return std::span(
            instance_to_view_translations_.cbegin(),
            static_cast<std::size_t>(num_active_layers)
        );
    }

    void swapLayerTextures(i32 src_texture_index, i32 dst_texture_index);
    void addLayerTexture();
    void addScale(f32 value);

    void deinit();

private:
    void sendInstanceToTextureTranslation(std::size_t index);
};

}

#endif