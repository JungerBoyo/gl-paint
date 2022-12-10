#ifndef GLPAINT_SHADER_HPP
#define GLPAINT_SHADER_HPP

#include <array>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include <common.hpp>

namespace glpaint {

struct Shader {
    enum class Type : usize { 
        COMPUTE 		= 1, 
        VERTEX_FRAGMENT = 2 
    };

	u32 prog_id_;
	std::array<u32, 2> shader_ids_;
    Type type_;

	Shader(Type type, const std::vector<std::filesystem::path>& paths);
	static std::vector<char> parseAsSpirv(const std::filesystem::path &path);
	static void compileShader(const std::filesystem::path &path, u32 shader_id);

	void bind() const;

	void deinit();
};

} // namespace rw_cube

#endif