#ifndef GLPAINT_STORAGE_BUFFER_OBJECT_HPP
#define GLPAINT_STORAGE_BUFFER_OBJECT_HPP

#include <common.hpp>

namespace glpaint {

struct StorageBufferObject {
    enum class Type : u32 {
        UBO,
        SSBO
    };

private:
    u32 id_{ 0 };
	i64 size_;
    void* mapped_ptr_{ nullptr };
public:
	StorageBufferObject(i64 size, u32 flags);

    void bind(Type type, u32 binding) const;
    void bind(Type type, u32 binding, i64 offset, i64 size) const;

	void sendData(const void *data) const;
	void sendData(const void *data, i64 offset, i64 size) const;

	void deinit();
};

}

#endif