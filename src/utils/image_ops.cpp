#include <image_ops.hpp>

#include <fstream>

#include <glad/glad.h>
#include <stb_image.h>
#include <stb_image_resize.h>

using namespace glpaint;

static std::optional<std::vector<char>> loadFileRaw(const fs::path& path);

std::optional<ImageData<u8>> loadImageU8(const fs::path& img_path, i32 desired_num_channels) {
    i32 width{ 0 };
    i32 height{ 0 };
    i32 num_channels{ 0 };
    u8* ptr{ nullptr };
    {
        auto img = loadFileRaw(img_path);
        if (!img.has_value()) {
            return std::nullopt;
        }
        ptr = stbi_load_from_memory(
            reinterpret_cast<const stbi_uc*>(img->data()), // NOLINT
            static_cast<i32>(img->size()),
            &width,
            &height,
            &num_channels,
            desired_num_channels
        );
    }
    if (ptr != nullptr) {
        std::vector<u8> data(static_cast<usize>(width * height * desired_num_channels));
        std::copy(ptr, ptr + data.size(), data.begin()); // NOLINT
        stbi_image_free(ptr);
        return ImageData<u8>{
            .data = std::move(data),
            .width = width,
            .height = height,
            .num_channels = desired_num_channels
        };
    } else {
        return std::nullopt;
    }
}
std::optional<ImageData<u16>> loadImageU16(const fs::path& img_path, i32 desired_num_channels) {
    i32 width{ 0 };
    i32 height{ 0 };
    i32 num_channels{ 0 };
    u16* ptr{ nullptr };
    {
        auto img = loadFileRaw(img_path);
        if (!img.has_value()) {
            return std::nullopt;
        }
        ptr = stbi_load_16_from_memory(
            reinterpret_cast<const stbi_uc*>(img->data()), // NOLINT
            static_cast<i32>(img->size()),
            &width,
            &height,
            &num_channels,
            desired_num_channels
        );
    }
    if (ptr != nullptr) {
        std::vector<u16> data(static_cast<usize>(width * height * desired_num_channels));
        std::copy(ptr, ptr + data.size(), data.begin()); // NOLINT
        return ImageData<u16>{
            .data = std::move(data),
            .width = width,
            .height = height,
            .num_channels = desired_num_channels
        };
    } else {
        return std::nullopt;
    }
}
std::optional<ImageData<f32>> loadImageF32(const fs::path& img_path, i32 desired_num_channels) {
    i32 width{ 0 };
    i32 height{ 0 };
    i32 num_channels{ 0 };
    f32* ptr{ nullptr };
    {
        auto img = loadFileRaw(img_path);
        if (!img.has_value()) {
            return std::nullopt;
        }
        ptr = stbi_loadf_from_memory(
            reinterpret_cast<const stbi_uc*>(img->data()), // NOLINT
            static_cast<i32>(img->size()),
            &width,
            &height,
            &num_channels,
            desired_num_channels
        );
    }
    if (ptr != nullptr) {
        std::vector<f32> data(static_cast<usize>(width * height * desired_num_channels));
        std::copy(ptr, ptr + data.size(), data.begin()); // NOLINT
        return ImageData<f32>{
            .data = std::move(data),
            .width = width,
            .height = height,
            .num_channels = desired_num_channels
        };
    } else {
        return std::nullopt;
    }
}

MipmapGenerator MipmapGenerator::create(
    const void* data, 
    i32 width, 
    i32 height, 
    i32 num_levels,
    i32 texel_size, 
    i32 num_channels,
    u32 texel_type) {

    i32 result_full_size{ 0 };
    i32 tmp_width{ width };
    i32 tmp_height{ height };
    for (i32 i{0}; i<num_levels; ++i) {
        result_full_size += tmp_height * tmp_width * num_channels * texel_size;
        tmp_width = std::max(1, tmp_width / 2);
        tmp_height = std::max(1, tmp_height / 2);
    }

    std::vector<u8> result(result_full_size);

    i32 result_i_read{ 0 };
    i32 result_i_write{ height * width * num_channels * texel_size };
    std::copy(
        static_cast<const u8*>(data), 
        static_cast<const u8*>(data) + result_i_write, // NOLINT
        result.begin()
    );

    co_yield {
        .data = static_cast<const void*>(result.data()),
        .width = width,
        .height = height
    };

    tmp_width = width;
    tmp_height = height;
    for (usize i{1}; i<num_levels; ++i) {
        const auto in_w = tmp_width;
        const auto in_h = tmp_height;
        const auto in_stride = in_w * num_channels * texel_size;

        tmp_width = std::max(1, tmp_width / 2);
        tmp_height = std::max(1, tmp_height / 2);
        const auto out_w = tmp_width;
        const auto out_h = tmp_height;

        const auto out_stride = out_w * num_channels * texel_size;   

        stbir_datatype datatype{ STBIR_TYPE_UINT8 };
        switch (texel_type) {
        case GL_UNSIGNED_BYTE:  datatype = STBIR_TYPE_UINT8;    break;
        case GL_UNSIGNED_SHORT: datatype = STBIR_TYPE_UINT16;   break;
        case GL_UNSIGNED_INT:   datatype = STBIR_TYPE_UINT32;   break;
        case GL_FLOAT:          datatype = STBIR_TYPE_FLOAT;    break;
        default: break;
        } 

        stbir_resize(
            result.data() + result_i_read, // NOLINT
            in_w, in_h, in_stride,
            result.data() + result_i_write, // NOLINT
            out_w, out_h, out_stride,
            datatype,
            num_channels,
            num_channels == 4 ? 3 : STBIR_ALPHA_CHANNEL_NONE,
            0,
            STBIR_EDGE_CLAMP, 
            STBIR_EDGE_CLAMP, 
            STBIR_FILTER_DEFAULT, 
            STBIR_FILTER_DEFAULT,
            STBIR_COLORSPACE_LINEAR,
            nullptr
        );

        co_yield {
            .data = static_cast<const void*>(result.data() + result_i_write),
            .width = out_w,
            .height = out_h  
        };

        const auto out_size = out_stride * out_h;
        result_i_read = result_i_write;
        result_i_write += out_size;
    }

    co_return;
}

std::optional<std::vector<char>> loadFileRaw(const fs::path& path) {
	std::ifstream stream(path.string(), std::ios::binary|std::ios::ate);
	if (!stream.good()) {
        return std::nullopt;
	}
	const auto size = static_cast<usize>(stream.tellg());
	std::vector<char> file(size);

	stream.seekg(0);
	stream.read(file.data(), static_cast<std::streamsize>(size));
	stream.close();
    
    return file;
}

