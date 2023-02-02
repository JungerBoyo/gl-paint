#include <StorageBufferObject.hpp>

#include <algorithm>
#include <cstring>

#include <glad/glad.h>

using namespace glpaint;

StorageBufferObject::StorageBufferObject(i64 size, u32 flags) : 
    size_(size) {
    glCreateBuffers(1, &id_);
    glNamedBufferStorage(id_, size_, nullptr, flags);
}

void StorageBufferObject::bind(Type type, u32 binding) const {
    glBindBufferBase(
        type == Type::UBO ? GL_UNIFORM_BUFFER : GL_SHADER_STORAGE_BUFFER,
        binding,
        id_
    );
}
void StorageBufferObject::bind(Type type, u32 binding, i64 offset, i64 size) const {
    const auto clamped_offset = std::clamp(offset, 0L, size_ - 1);
    const auto clamped_size = std::clamp(size, 1L, size_ - clamped_offset);
    glBindBufferRange(
        type == Type::UBO ? GL_UNIFORM_BUFFER : GL_SHADER_STORAGE_BUFFER,
        binding,
        id_,
        clamped_offset,
        clamped_size
    );
}
void StorageBufferObject::sendData(const void *data) const {
    if (mapped_ptr_ == nullptr) {
        glNamedBufferSubData(id_, 0L, size_, data);
    } else {
        std::memcpy(mapped_ptr_, data, size_);
    }
}
void StorageBufferObject::sendData(const void *data, i64 offset, i64 size) const {
    const auto clamped_offset = std::clamp(offset, 0L, size_ - 1);
    const auto clamped_size = std::clamp(size, 1L, size_ - clamped_offset);
    if (mapped_ptr_ == nullptr) {
        glNamedBufferSubData(id_, clamped_offset, clamped_size, data);
    } else {
        std::memcpy(
            static_cast<void*>(
                static_cast<u8*>(mapped_ptr_) + clamped_offset // NOLINT
            ),
            data,
            clamped_size
        );
    }
}
void StorageBufferObject::deinit() {
    glDeleteBuffers(1, &id_);
    size_ = 0;
    id_ = 0;
}