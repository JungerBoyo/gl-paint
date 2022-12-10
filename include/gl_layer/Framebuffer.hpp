#ifndef GLPAINT_FRAMEBUFFER_HPP
#define GLPAINT_FRAMEBUFFER_HPP

#include <vector>

#include <common.hpp>

namespace glpaint {

struct Framebuffer {
    struct ColorAttachment {
        u32 attachment_id;
        u32 internal_format;
    };
private:
    u32 fbo_id_{ 0 };
    std::vector<ColorAttachment> attachments_;
    std::vector<u32> textures_;
    i32 width_;
    i32 height_;

public:
    Framebuffer(const std::vector<ColorAttachment>& attachments, i32 w, i32 h);

    void tryResize(i32 new_w, i32 new_h);

    void bind();
    void unbind();

    [[nodiscard]] auto id() const { return fbo_id_; }
    [[nodiscard]] auto textureId(usize index) const { return textures_[index]; }

    void deinit();
};

}

#endif