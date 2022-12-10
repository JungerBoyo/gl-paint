#include <Quad.hpp>

#include <glad/glad.h>

using namespace glpaint;

Quad::Quad() {
    glCreateBuffers(1, &vbo_id_);
    glNamedBufferStorage(
        vbo_id_,
        sizeof(VERTICES),
        static_cast<const void*>(VERTICES.data()),
        0
    );
    glCreateVertexArrays(1, &vao_id_);
    constexpr std::uint32_t attribs_binding{ 0 };
    constexpr std::array<std::uint32_t, 2> attrib_locations{{
        SHCONFIG_IN_POSITION_LOCATION,
        SHCONFIG_IN_TEXCOORD_LOCATION
    }};
    constexpr std::uint32_t base_size{ sizeof(float) };
    constexpr std::uint32_t num_components{ 2 };
    std::uint32_t relative_offset{ 0 };
    for (const auto attrib_location : attrib_locations) {
        glEnableVertexArrayAttrib(vao_id_, attrib_location);
        glVertexArrayAttribFormat(
            vao_id_, 
            attrib_location,
            num_components,
            GL_FLOAT,
            GL_FALSE,
            relative_offset
        );
        glVertexArrayAttribBinding(vao_id_, attrib_location, attribs_binding);
        relative_offset += num_components * base_size;
    }
    glVertexArrayBindingDivisor(vao_id_, attribs_binding, 0);
    glVertexArrayVertexBuffer(
        vao_id_, 
        attribs_binding, 
        vbo_id_, 0, 
        static_cast<int>(relative_offset)
    );

}

void Quad::bind() const {
    glBindVertexArray(vao_id_);
}

void Quad::deinit() {
    glDeleteBuffers(1, &vbo_id_);
    glDeleteVertexArrays(1, &vao_id_);
}