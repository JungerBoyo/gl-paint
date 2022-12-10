#ifndef GLPAINT_TEXTURE_2D_HPP
#define GLPAINT_TEXTURE_2D_HPP

#include <common.hpp>
#include <tuple>

namespace glpaint {

struct Texture2D {
    struct Config {
        i32 width;
        i32 height;
        u32 internal_format;
        u32 texel_data_format;
        u32 texel_type;
    };

private:
    u32 tex_id_{ 0 };
    f32 offset_[2]{ 0.F, 0.F }; // offset from left bottom cornet of the image
    f32 zoom_[2]{ 1.F, 1.F };   // scale of the image from 0 to 1

public:
    Texture2D(const void* pixel_data, Config config);
    void bind(u32 unit) const;
    void deinit();

    auto addOffset(f32 x, f32 y) {
        offset_[0] += x;
        offset_[1] += y;
        return std::make_tuple(offset_[0], offset_[1]);
    }
    auto addZoom(f32 x, f32 y) {
        zoom_[0] += x;
        zoom_[1] += y;
        return std::make_tuple(zoom_[0], zoom_[1]);
    }
    auto addZoom(f32 zoom) {
        zoom_[0] += zoom;
        zoom_[1] += zoom;
        return std::make_tuple(zoom_[0], zoom_[1]);
    }
    [[nodiscard]] auto zoom() const { 
        return std::make_tuple(zoom_[0], zoom_[1]);
    }
    [[nodiscard]] auto offset() const { 
        return std::make_tuple(offset_[0], offset_[1]);
    }
};

}

#endif