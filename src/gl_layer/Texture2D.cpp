#include <Texture2D.hpp>

#include <glad/glad.h>

#include <cmath>

using namespace glpaint;

Texture2D::Texture2D(const void* pixel_data, Config config) {
    glCreateTextures(GL_TEXTURE_2D, 1, &tex_id_);

    const auto num_levels = 1 + static_cast<i32>(std::log2(std::max(
        static_cast<f32>(config.width), 
        static_cast<f32>(config.height)
    )));
    glTextureStorage2D(
        tex_id_, 
        num_levels, 
        config.internal_format, 
        config.width, 
        config.height
    );
    glTextureSubImage2D(
        tex_id_, 
        0, 0, 0, config.width, config.height, 
        config.texel_data_format,
        config.texel_type, 
        pixel_data
    );
    glGenerateTextureMipmap(tex_id_);
}
void Texture2D::bind(u32 unit) const {
    glBindTextureUnit(unit, tex_id_);
}
void Texture2D::deinit() {
    glDeleteTextures(1, &tex_id_);
    tex_id_ = 0;
}