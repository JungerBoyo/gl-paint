#include <Texture2DArray.hpp>

#include <cmath>
#include <tuple>
#include <vector>

#include <image_ops.hpp>
#include <glad/glad.h>

using namespace glpaint;

static std::tuple<i32, i32> getTexelInfo(u32 internal_format);

Texture2DArray::Texture2DArray(Config config, bool create_texture_views) :
    config_(config) {

    glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &tex_array_id_);

    num_levels_ = 1 + static_cast<i32>(std::log2(std::max(
        static_cast<f32>(config.width), 
        static_cast<f32>(config.height)
    )));
    glTextureStorage3D(
        tex_array_id_, 
        num_levels_, 
        config.internal_format, 
        config.width, 
        config.height,
        config.count
    );

    if (create_texture_views) {
        tex_array_view_ids_.resize(config.count, 0U);
        glGenTextures(config.count, tex_array_view_ids_.data());
        u32 i{0};
        for (const auto view_id : tex_array_view_ids_) {
            glTextureView(
                view_id, 
                GL_TEXTURE_2D,
                tex_array_id_,
                config.internal_format,
                0, 1,
                i, 1
            );
            ++i;
        }
    }
}
void Texture2DArray::loadData(i32 image_index, const void* data, bool gen_mipmap) {
    if (image_index < config_.count) {
        if (gen_mipmap) {
            const auto[type_size, num_channels] = getTexelInfo(config_.internal_format);
            auto generator = MipmapGenerator::create(
                data,
                config_.width,
                config_.height,
                num_levels_,
                type_size,
                num_channels,
                config_.texel_type 
            );
            for (i32 i{0}; i<num_levels_; ++i) {
                const auto next_level = generator.nextMipmapLevel();
                glTextureSubImage3D(
                    tex_array_id_,
                    i,
                    0, 0, image_index,
                    next_level.width, next_level.height, 1,
                    config_.texel_data_format,
                    config_.texel_type,
                    next_level.data
                );  
            }
        } else {
            glTextureSubImage3D(
                tex_array_id_,
                0,
                0, 0, image_index,
                config_.width, config_.height, 1,
                config_.texel_data_format,
                config_.texel_type,
                data
            );  
        }
    }
}

void Texture2DArray::copyData(i32 dst_image_index, i32 src_image_index, bool with_mipmap) {
    if ((dst_image_index != src_image_index) &&
        (dst_image_index >= 0 && dst_image_index < config_.count) && 
        (src_image_index >= 0 && src_image_index < config_.count)) {
        glCopyImageSubData(
            // src
            tex_array_id_, GL_TEXTURE_2D_ARRAY,
            0, 0, 0, src_image_index, 
            // dst
            tex_array_id_, GL_TEXTURE_2D_ARRAY,
            0, 0, 0, dst_image_index,
            // src again
            config_.width, config_.height, 1
        );
        if (with_mipmap) {
            auto tmp_width = std::max(1, config_.width/2);
            auto tmp_height = std::max(1, config_.height/2);
            for (i32 i{1}; i<num_levels_; ++i) {
                glCopyImageSubData(
                    // src
                    tex_array_id_, GL_TEXTURE_2D_ARRAY,
                    i, 0, 0, src_image_index, 
                    // dst
                    tex_array_id_, GL_TEXTURE_2D_ARRAY,
                    i, 0, 0, dst_image_index,
                    // src again
                    tmp_width, tmp_height, 1
                );
                tmp_width = std::max(1, tmp_width/2);
                tmp_height = std::max(1, tmp_width/2);
            }
        }
    }
}


void Texture2DArray::resize(i32 count) {
    if (count > 0 && count != config_.count) {
        u32 tmp_id{ 0 };
        glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &tmp_id);
        glTextureStorage3D(
            tmp_id, 
            num_levels_, 
            config_.internal_format, 
            config_.width, 
            config_.height,
            count
        );

        const auto to_copy_count = std::min(count, config_.count);
        auto tmp_width = config_.width;
        auto tmp_height = config_.height;
        for (i32 l{0}; l<num_levels_; ++l) {
            glCopyImageSubData(
                // src
                tex_array_id_, GL_TEXTURE_2D_ARRAY,
                l, 0, 0, 0, 
                // dst
                tmp_id, GL_TEXTURE_2D_ARRAY,
                l, 0, 0, 0,
                // src again
                tmp_width, tmp_height, to_copy_count 
            );
            tmp_width = std::max(1, tmp_width/2);
            tmp_height = std::max(1, tmp_width/2);
        }

        glDeleteTextures(
            static_cast<i32>(tex_array_view_ids_.size()),
            tex_array_view_ids_.data()
        );
        tex_array_view_ids_.resize(count, 0U);
        glGenTextures(count, tex_array_view_ids_.data());
        u32 i{0};
        for (const auto view_id : tex_array_view_ids_) {
            glTextureView(
                view_id, 
                GL_TEXTURE_2D,
                tmp_id,
                config_.internal_format,
                0, 1,
                i, 1
            );
            ++i;
        }
        
        glDeleteTextures(1, &tex_array_id_);
        tex_array_id_ = tmp_id;
        config_.count = count;
    }
}

void Texture2DArray::bind(u32 unit) const {
    glBindTextureUnit(unit, tex_array_id_);
}

void Texture2DArray::deinit() {
    glDeleteTextures(1, &tex_array_id_);
    tex_array_id_ = 0;
}

std::tuple<i32, i32> getTexelInfo(u32 internal_format) {
  switch (internal_format) {
    case GL_R8:         return { 1, 1 };
    case GL_RG8:        return { 1, 2 };
    case GL_RGB8:       return { 1, 3 };
    case GL_RGBA8:      return { 1, 4 };
    case GL_R16F:       return { 2, 1 };
    case GL_RG16F:      return { 2, 2 };
    case GL_RGB16F:     return { 2, 3 };
    case GL_RGBA16F:    return { 2, 4 };
    case GL_R32F:       return { 4, 1 };
    case GL_RG32F:      return { 4, 2 };
    case GL_RGB32F:     return { 4, 3 };
    case GL_RGBA32F:    return { 4, 4 };
    case GL_R8I:        return { 1, 1 };
    case GL_RG8I:       return { 1, 2 };
    case GL_RGB8I:      return { 1, 3 };
    case GL_RGBA8I:     return { 1, 4 };
    case GL_R16I:       return { 2, 1 };
    case GL_RG16I:      return { 2, 2 };
    case GL_RGB16I:     return { 2, 3 };
    case GL_RGBA16I:    return { 2, 4 };
    case GL_R32I:       return { 4, 1 };
    case GL_RG32I:      return { 4, 2 };
    case GL_RGB32I:     return { 4, 3 };
    case GL_RGBA32I:    return { 4, 4 };
    case GL_R8UI:       return { 1, 1 };
    case GL_RG8UI:      return { 1, 2 };
    case GL_RGB8UI:     return { 1, 3 };
    case GL_RGBA8UI:    return { 1, 4 };
    case GL_R16UI:      return { 2, 1 };
    case GL_RG16UI:     return { 2, 2 };
    case GL_RGB16UI:    return { 2, 3 };
    case GL_RGBA16UI:   return { 2, 4 };
    case GL_R32UI:      return { 4, 1 };
    case GL_RG32UI:     return { 4, 2 };
    case GL_RGB32UI:    return { 4, 3 };
    case GL_RGBA32UI:   return { 4, 4 };
    // additional cases for other internal formats
    default: return { 0, 0 }; break;
  }
}