#ifndef GLPAINT_TEXTURE_2D_ARRAY_HPP
#define GLPAINT_TEXTURE_2D_ARRAY_HPP

#include <common.hpp>

namespace glpaint {

struct Texture2DArray {
    struct Config {
        i32 width;
        i32 height;
        i32 count;
        u32 internal_format;
        u32 texel_data_format;
        u32 texel_type;
    };
private:
    Config config_;
    i32 num_levels_{ 0 };
    u32 tex_array_id_{ 0 };

public:
    Texture2DArray(Config config);

    void loadData(i32 image_index, const void* data);
    void addImage(i32 growth_factor, const void* data = nullptr);

    void bind(u32 unit) const;

    void deinit();
};

}

#endif