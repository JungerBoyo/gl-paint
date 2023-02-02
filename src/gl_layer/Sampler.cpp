#include <Sampler.hpp>

#include <glad/glad.h>

using namespace glpaint;

Sampler::Sampler(Config config) {
    glCreateSamplers(1, &sampler_id_);

    if (config.texture_wrap_s.has_value()) {
        glSamplerParameteri(
            sampler_id_, 
            GL_TEXTURE_WRAP_S, 
            config.texture_wrap_s.value()
        );
    }
    if (config.texture_wrap_t.has_value()) {
        glSamplerParameteri(
            sampler_id_, 
            GL_TEXTURE_WRAP_T, 
            config.texture_wrap_t.value()
        );
    }
    if (config.texture_min_filter.has_value()) {
        glSamplerParameteri(
            sampler_id_, 
            GL_TEXTURE_MIN_FILTER, 
            config.texture_min_filter.value()
        );
    }
    if (config.texture_mag_filter.has_value()) {
        glSamplerParameteri(
            sampler_id_, 
            GL_TEXTURE_MAG_FILTER, 
            config.texture_mag_filter.value()
        );
    }
}

void Sampler::bind(u32 unit) const {
    glBindSampler(unit, sampler_id_);
}

void Sampler::deinit() {
    glDeleteSamplers(1, &sampler_id_);
    sampler_id_ = 0;
}