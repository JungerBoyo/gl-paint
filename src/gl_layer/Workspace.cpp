#include <Workspace.hpp>
#include <GLLayer.hpp>
#include <Quad.hpp>

#include <image_ops.hpp>

#include <glad/glad.h>

using namespace glpaint;

Workspace::Workspace(i32 workspace_w, i32 workspace_h) :
    workspace_tex2d_array_({
        workspace_w,
        workspace_h,
        INITIAL_CAPACITY,
        GL_RGBA8,
        GL_RGBA,
        GL_UNSIGNED_BYTE
    }, true),
    basic_tex2d_sampler_({
        GL_NEAREST,
        GL_NEAREST,
        GL_REPEAT,
        GL_REPEAT
    }),
    workspace_ubo_(
        sizeof(Transform) + MAX_CAPACITY * sizeof(InstanceToTextureTranslation), 
        GL_DYNAMIC_STORAGE_BIT
    ),
    workspace_w_(workspace_w),
    workspace_h_(workspace_h),
    instance_to_texture_translations_({{ 0 }, { 1 }, { 2 }, { 3 }}),
    instance_to_view_translations_(INITIAL_CAPACITY - 1) {

    // const auto img = loadImageU8("assets/textures/Transparency10-alpha0.png", 4);

    // if (img.has_value()) {
        // const auto fitted_img = repeatPattern<u8>(img.value(), workspace_w, workspace_h);

        // for (auto i{SANDBOX_TEX2D_INDEX}; i<INITIAL_CAPACITY; ++i) {
        //     workspace_tex2d_array_.loadData(
        //         i, 
        //         static_cast<const void*>(fitted_img.data.data()),
        //         false
        //     );
        // }
    // } else {
    //     throw std::runtime_error("failed to load base transparent texture");
    // }

    workspace_tex2d_array_.bind(SHCONFIG_WORKSPACE_TEXTURE_ARRAY_BINDING);

    const auto& views = workspace_tex2d_array_.views();
    std::copy(
        std::next(views.cbegin(), 1),
        views.cend(),
        instance_to_view_translations_.begin()
    );


    workspace_ubo_.sendData(
        static_cast<const void*>(&workspace_transform_),
        0,
        sizeof(Transform)
    );
    workspace_ubo_.sendData(
        static_cast<const void*>(instance_to_texture_translations_.data()),
        sizeof(Transform),
        static_cast<i64>(
            sizeof(InstanceToTextureTranslation) * instance_to_texture_translations_.size()
        )
    );
    workspace_ubo_.bind(
        StorageBufferObject::Type::UBO,
        SHCONFIG_WOKRSPACE_TRANSFORM_UBO_BINDING
    );
}
void Workspace::update() {
    GLLayer::shaderRepository()->workspaceViewportShader().bind();
    basic_tex2d_sampler_.bind(SHCONFIG_WORKSPACE_TEXTURE_ARRAY_BINDING);
    glDrawArraysInstancedBaseInstance(
        GL_TRIANGLES,
        0,
        Quad::VERTICES_COUNT,
        num_active_layers,
        1
    );
}
void Workspace::swapLayerTextures(i32 src_texture_index, i32 dst_texture_index) {
    // first layer is sandbox layer invisible to user
    if (dst_texture_index != src_texture_index) {
        // src_texture_index += 1;
        // dst_texture_index += 1;
        if ((src_texture_index >= 0 && src_texture_index < num_active_layers) &&
            (dst_texture_index >= 0 && dst_texture_index < num_active_layers)) {
            
            std::swap(
                instance_to_texture_translations_[src_texture_index + 1],
                instance_to_texture_translations_[dst_texture_index + 1]
            );
            sendInstanceToTextureTranslation(src_texture_index + 1);
            sendInstanceToTextureTranslation(dst_texture_index + 1);

            std::swap(
                instance_to_view_translations_[src_texture_index],
                instance_to_view_translations_[dst_texture_index]
            );
        }
    }
}
void Workspace::addLayerTexture() {
    const auto new_raw_num_layers{ 1 + (num_active_layers + 1) };
    if (new_raw_num_layers <= MAX_CAPACITY) {
        if (new_raw_num_layers > workspace_tex2d_array_.size()) {
            workspace_tex2d_array_.resize(new_raw_num_layers);
            workspace_tex2d_array_.bind(SHCONFIG_WORKSPACE_TEXTURE_ARRAY_BINDING);
            
            instance_to_texture_translations_.push_back(InstanceToTextureTranslation{
                static_cast<u32>(num_active_layers + 1)
            });

            instance_to_view_translations_.push_back(0); // dummy

            const auto& views = workspace_tex2d_array_.views();
            for (std::size_t i{1}; i<new_raw_num_layers; ++i) {
                instance_to_view_translations_[i - 1] = 
                    views[instance_to_texture_translations_[i].texture_layer_index];
            }

            // workspace_tex2d_array_.copyData(
            //     num_active_layers + 1,
            //     SANDBOX_TEX2D_INDEX,
            //     false
            // );

            sendInstanceToTextureTranslation(num_active_layers + 1);
        }
        ++num_active_layers;
    }
}
void Workspace::addScale(f32 value) {
    static constexpr f32 step{ 0.015F };

    const auto new_scale = workspace_transform_.scale + step * value;
    workspace_transform_.scale = std::clamp(new_scale, 0.F, 20.F);
    workspace_ubo_.sendData(
        static_cast<const void*>(
            reinterpret_cast<u8*>(&workspace_transform_) + offsetof(Transform, scale)
        ),
        offsetof(Transform, scale),
        sizeof(f32)
    );
}

void Workspace::deinit() {
    workspace_tex2d_array_.deinit();
}

void Workspace::sendInstanceToTextureTranslation(std::size_t index) {
    workspace_ubo_.sendData(
        static_cast<const void*>(&instance_to_texture_translations_[index]),
        sizeof(Transform) + index * sizeof(InstanceToTextureTranslation),
        static_cast<i64>(sizeof(InstanceToTextureTranslation))
    );
}