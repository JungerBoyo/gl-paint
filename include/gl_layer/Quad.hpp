#ifndef GLPAINT_QUAD_HPP
#define GLPAINT_QUAD_HPP

#include <array>

#include <common.hpp>

namespace glpaint {

struct Quad {
    static constexpr std::array<float, 6UL*(2UL+2UL)> VERTICES {{
        -1.F,-1.F, 0.F, 0.F,
         1.F,-1.F, 1.F, 0.F,
         1.F, 1.F, 1.F, 1.F,
        -1.F,-1.F, 0.F, 0.F,
         1.F, 1.F, 1.F, 1.F,
        -1.F, 1.F, 0.F, 1.F,
    }};
private:
    u32 vbo_id_{ 0 };
    u32 vao_id_{ 0 };

public:
    Quad();
    void bind() const;
    void deinit();
};

}

#endif