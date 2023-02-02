#ifndef GLPAINT_IMAGE_OPS_HPP
#define GLPAINT_IMAGE_OPS_HPP

#include <common.hpp>

#include <vector>
#include <filesystem>
#include <optional>
#include <coroutine>
#include <utility>

namespace fs = std::filesystem;

namespace glpaint {
    template<typename T>
    requires std::is_unsigned_v<T> || std::is_floating_point_v<T>
    struct ImageData {
        std::vector<T> data;
        i32 width{ 0 };
        i32 height{ 0 };
        i32 num_channels{ 0 };
    };
    std::optional<ImageData<u8>> loadImageU8(const fs::path& img_path, i32 desired_num_channels);
    std::optional<ImageData<u16>> loadImageU16(const fs::path& img_path, i32 desired_num_channels);
    std::optional<ImageData<f32>> loadImageF32(const fs::path& img_path, i32 desired_num_channels);

    template<typename T>
    ImageData<T> repeatPattern(const ImageData<T>& src_image, i32 dst_width, i32 dst_height) {
        const auto full_size = dst_width * dst_height * src_image.num_channels;
        std::vector<T> dst_data(full_size);

        const auto src_width = src_image.width;
        const auto src_height = src_image.height;
        const auto src_num_channels = src_image.num_channels;
        const auto src_stride = src_num_channels * src_width;
        const auto dst_stride = src_num_channels * dst_width;
        for (usize y{0}; y < dst_height; ++y) {
            for (usize x{0}; x < dst_stride; ++x) {
                dst_data[x + y * dst_stride] = src_image.data[(x % src_stride) + (y % src_height) * src_stride];
            }
        }
        return {
            std::move(dst_data),
            dst_width,
            dst_height,
            src_num_channels
        };
    }


    struct MipmapGenerator {
        struct MipmapLevelData {
            const void* data{ nullptr };
            i32 width{ 0 }; 
            i32 height{ 0 }; 
        };
        struct promise_type {
            MipmapLevelData out_mipmap_lvl_data_{};

            void unhandled_exception() noexcept {}
            MipmapGenerator get_return_object() { return MipmapGenerator{this}; }
            std::suspend_always initial_suspend() noexcept { return {}; }
            std::suspend_always yield_value(MipmapLevelData value) noexcept {
                out_mipmap_lvl_data_ = std::move(value);
                return {};
            }
            std::suspend_always await_transform(MipmapLevelData) noexcept { 
                return {};
            }
            void return_void() noexcept {}
            std::suspend_always final_suspend() noexcept { return {}; }
        };
    private:
        using CoroHandle = std::coroutine_handle<promise_type>;
        CoroHandle coro_handle_{};

    public:
        explicit MipmapGenerator(promise_type* p) :
            coro_handle_(CoroHandle::from_promise(*p)) {}
        MipmapGenerator(MipmapGenerator&& other) :
            coro_handle_{std::exchange(other.coro_handle_, nullptr)} {}
        ~MipmapGenerator() {
            if (coro_handle_) {
                coro_handle_.destroy();
            }
        }

        auto nextMipmapLevel() {
            if (!coro_handle_.done()) {
                coro_handle_.resume();
            }
            return std::move(coro_handle_.promise().out_mipmap_lvl_data_);
        }

        static MipmapGenerator create(
            const void* data, 
            i32 width, 
            i32 height, 
            i32 num_levels,
            i32 texel_size, 
            i32 num_channels,
            u32 texel_type
        );
    };
}

#endif