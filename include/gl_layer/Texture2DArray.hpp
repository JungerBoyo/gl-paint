#ifndef GLPAINT_TEXTURE_2D_ARRAY_HPP
#define GLPAINT_TEXTURE_2D_ARRAY_HPP

#include <vector>

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
    std::vector<u32> tex_array_view_ids_;

public:
    Texture2DArray(Config config, bool create_texture_views);

    void loadData(i32 image_index, const void* data, bool gen_mipmap);
    void copyData(i32 dst_image_index, i32 src_image_index, bool with_mipmap);

    void resize(i32 count);

    [[nodiscard]] auto size() const { return config_.count; }
    [[nodiscard]] const auto& views() const { return tex_array_view_ids_; }

    void bind(u32 unit) const;

    void deinit();
};

}

#endif