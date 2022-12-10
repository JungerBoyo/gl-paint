#include <Texture2DArray.hpp>

#include <cmath>
#include <tuple>
#include <vector>

#include <image_ops.hpp>
#include <glad/glad.h>

using namespace glpaint;

static std::tuple<i32, i32> getTexelInfo(u32 internal_format);

Texture2DArray::Texture2DArray(Config config) :
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
}
void Texture2DArray::loadData(i32 image_index, const void* data) {
    if (image_index < config_.count) {
        const auto[texel_size, num_channels] = getTexelInfo(config_.internal_format);
        auto generator = MipmapGenerator::create(
            data,
            config_.width,
            config_.height,
            num_levels_,
            texel_size,
            num_channels,
            config_.texel_type 
        );
        for (i32 i{0}; i<num_levels_; ++i) {
            const auto next_level = generator.nextMipmapLevel();
            glTextureSubImage3D(
               tex_array_id_,
               i,
               0, 0, 0,
               next_level.width, next_level.height, image_index,
               config_.texel_data_format,
               config_.texel_type,
               next_level.data
            );  
        }
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
    case GL_RG8:        return { 2, 2 };
    case GL_RGB8:       return { 3, 3 };
    case GL_RGBA8:      return { 4, 4 };
    case GL_R16F:       return { 2, 1 };
    case GL_RG16F:      return { 4, 2 };
    case GL_RGB16F:     return { 6, 3 };
    case GL_RGBA16F:    return { 8, 4 };
    case GL_R32F:       return { 4, 1 };
    case GL_RG32F:      return { 8, 2 };
    case GL_RGB32F:     return { 12, 3 };
    case GL_RGBA32F:    return { 16, 4 };
    case GL_R8I:        return { 1, 1 };
    case GL_RG8I:       return { 2, 2 };
    case GL_RGB8I:      return { 3, 3 };
    case GL_RGBA8I:     return { 4, 4 };
    case GL_R16I:       return { 2, 1 };
    case GL_RG16I:      return { 4, 2 };
    case GL_RGB16I:     return { 6, 3 };
    case GL_RGBA16I:    return { 8, 4 };
    case GL_R32I:       return { 4, 1 };
    case GL_RG32I:      return { 8, 2 };
    case GL_RGB32I:     return { 12, 3 };
    case GL_RGBA32I:    return { 16, 4 };
    case GL_R8UI:       return { 1, 1 };
    case GL_RG8UI:      return { 2, 2 };
    case GL_RGB8UI:     return { 3, 3 };
    case GL_RGBA8UI:    return { 4, 4 };
    case GL_R16UI:      return { 2, 1 };
    case GL_RG16UI:     return { 4, 2 };
    case GL_RGB16UI:    return { 6, 3 };
    case GL_RGBA16UI:   return { 8, 4 };
    case GL_R32UI:      return { 4, 1 };
    case GL_RG32UI:     return { 8, 2 };
    case GL_RGB32UI:    return { 12, 3 };
    case GL_RGBA32UI:   return { 16, 4 };
    // additional cases for other internal formats
    default: return { 0, 0 }; break;
  }
}