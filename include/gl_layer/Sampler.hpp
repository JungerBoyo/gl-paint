#ifndef GLPAINT_SAMPLER_HPP
#define GLPAINT_SAMPLER_HPP

#include <optional>
#include <common.hpp>

namespace glpaint {

struct Sampler {
    struct Config {
        std::optional<i32> texture_wrap_s; 
        std::optional<i32> texture_wrap_t; 
        std::optional<i32> texture_min_filter; 
        std::optional<i32> texture_mag_filter; 
    };
private:
    u32 sampler_id_{ 0 };

public:
    Sampler(Config config);

    void bind(u32 unit) const;

    void deinit();
};

}

#endif