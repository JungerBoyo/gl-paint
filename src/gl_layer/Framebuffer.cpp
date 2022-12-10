#include <Framebuffer.hpp>

#include <glad/glad.h>

using namespace glpaint;

Framebuffer::Framebuffer(const std::vector<ColorAttachment>& attachments, i32 w, i32 h) :
    textures_(attachments_.size(), 0), attachments_(attachments), width_(w), height_(h) {
    glCreateFramebuffers(1, &fbo_id_);
    
    glCreateTextures(
        GL_TEXTURE_2D, 
        static_cast<i32>(textures_.size()), 
        textures_.data()
    );

    glBindFramebuffer(GL_FRAMEBUFFER, fbo_id_);
    usize i{0};
    for (const auto texture : textures_) {
        glTextureStorage2D(
            texture,
            1, attachments_[i].internal_format,
            w, h
        );
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, 
            GL_COLOR_ATTACHMENT0 + attachments_[i].attachment_id,
            GL_TEXTURE_2D,
            texture,
            0
        );
        ++i;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void Framebuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_id_);
}
void Framebuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void Framebuffer::tryResize(i32 new_w, i32 new_h) {
    if (new_w == width_ && new_h == height_) {
        return;
    }
    glDeleteTextures(static_cast<i32>(textures_.size()), textures_.data());

    glBindFramebuffer(GL_FRAMEBUFFER, fbo_id_);
    usize i{0};
    for (const auto texture : textures_) {
        glTextureStorage2D(
            texture,
            1, attachments_[i].internal_format,
            new_w, new_h
        );
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, 
            GL_COLOR_ATTACHMENT0 + i,
            GL_TEXTURE_2D,
            texture,
            0
        );
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    width_ = new_w;
    height_ = new_h;
}
void Framebuffer::deinit() {
    glDeleteTextures(static_cast<i32>(textures_.size()), textures_.data());
    glDeleteFramebuffers(1, &fbo_id_);
}