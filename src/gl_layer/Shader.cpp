#include <Shader.hpp>

#include <exception>
#include <fstream>

#include <spdlog/spdlog.h>
#include <glad/glad.h>

using namespace glpaint;

Shader::Shader(Type type, const std::vector<std::filesystem::path>& paths) : type_(type) {
    i32 ext_num{0};
    bool is_arb_spirv_supported{false};
    glGetIntegerv(GL_NUM_EXTENSIONS, &ext_num);
    for (i32 i{0}; i<ext_num; ++i) {
        const auto ext_str = std::string_view(reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i))); // NOLINT
        if (ext_str == "GL_ARB_gl_spirv") {
            is_arb_spirv_supported = true;
            break;
        }
    }

    const auto sh_count = static_cast<usize>(type);
    if (paths.size() != sh_count) {
        throw std::runtime_error(spdlog::fmt_lib::format(
            "Wrong number of shaders for shader type {}. passed {} must be {}", 
            type == Type::VERTEX_FRAGMENT ? "VERTEX_FRAGMENT" : "COMPUTE",
			paths.size(),
            sh_count
        ));
    }
    const auto enums = type == Type::VERTEX_FRAGMENT ? 
	std::vector<u32>({
        GL_VERTEX_SHADER,
        GL_FRAGMENT_SHADER
    }) : 
	std::vector<u32>({
        GL_COMPUTE_SHADER
    });

    u32 is_spirv{ 0 };
    for (const auto& path : paths) {
        is_spirv += static_cast<u32>(path.extension().string() == ".spv");
    }
    if (is_spirv != 0 && is_spirv != sh_count) {
        throw std::runtime_error("Spirv shaders and runtime compiled shaders can't be combined");
    } 
    if (is_spirv == sh_count && !is_arb_spirv_supported) {
        throw std::runtime_error("Tried to pass spirv shaders but spirv shaders aren't supported on this GPU");
    }

    const bool use_spirv = is_spirv > 0 && is_arb_spirv_supported;

	prog_id_ = glCreateProgram();

	for (usize i{0}; i < 2; ++i) {
		auto& sh_id = shader_ids_.at(i);
        sh_id = glCreateShader(enums[i]);
		if (use_spirv) {
			const auto sh_binary = parseAsSpirv(paths[i]);
			glShaderBinary(1, static_cast<const GLuint *>(&sh_id),
						GL_SHADER_BINARY_FORMAT_SPIR_V_ARB,
						static_cast<const void *>(sh_binary.data()),
						static_cast<GLsizei>(sh_binary.size()));
			glSpecializeShaderARB(sh_id, "main", 0, nullptr, nullptr);
		} else {
			compileShader(paths[i], sh_id);
		}
		glAttachShader(prog_id_, sh_id);
	}

	glValidateProgram(prog_id_);
	glLinkProgram(prog_id_);
}

std::vector<char> Shader::parseAsSpirv(const std::filesystem::path &path) {
	std::ifstream stream(path.c_str(), std::ios::binary | std::ios::ate);

	if (!stream.good()) {
		throw std::runtime_error(spdlog::fmt_lib::format("failed to create stream from shader file {}", path.string()));
	}

	const auto size = static_cast<usize>(stream.tellg());
	std::vector<char> code(size);

	stream.seekg(0);
	stream.read(code.data(), static_cast<std::streamsize>(size));

	stream.close();

	return code;
}

void Shader::compileShader(const std::filesystem::path &path, u32 shader_id) {
	std::ifstream stream(path.c_str());
	std::stringstream sstream;
	for (std::string line; std::getline(stream, line);) {
		sstream << line << '\n';
	}
	const auto shader_src = sstream.str();

	const char* shader_src_cstr = shader_src.c_str();
	const auto len = static_cast<i32>(shader_src.length());

	glShaderSource(shader_id, 1, &shader_src_cstr, &len);
	glCompileShader(shader_id);
}
void Shader::bind() const {
	glUseProgram(prog_id_);
}

void Shader::deinit() {
	for (usize i{0}; i < 2; ++i) {
		auto &sh_id = shader_ids_.at(i);
		glDetachShader(prog_id_, sh_id);
		glDeleteShader(sh_id);
		sh_id = 0;
	}
	glDeleteProgram(prog_id_);
	prog_id_ = 0;
}