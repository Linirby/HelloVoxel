#include <fstream>

#include "render/core/shader.hpp"

namespace lili {

Shader::Shader(
	SDL_GPUDevice *device,
	const std::string &vert_path,
	const std::string &frag_path,
	ShaderInfo vert_infos,
	ShaderInfo frag_infos
) {
	this->device = device;
	CodeInfo vertex_code_info = get_code_info(vert_path);
	SDL_GPUShaderCreateInfo vertex_create_info{
		.code_size = vertex_code_info.size,
		.code = reinterpret_cast<uint8_t *>(vertex_code_info.buffer.data()),
		.entrypoint = "main",
		.format = SDL_GPU_SHADERFORMAT_SPIRV,
		.stage = SDL_GPU_SHADERSTAGE_VERTEX,
		.num_samplers = vert_infos.num_samplers,
		.num_storage_textures = vert_infos.num_storage_textures,
		.num_storage_buffers = vert_infos.num_storage_buffers,
		.num_uniform_buffers = vert_infos.num_uniform_buffers,
		.props = 0
	};
	vertex_shader = SDL_CreateGPUShader(this->device, &vertex_create_info);
	if (!vertex_shader) {
		throw std::runtime_error(
			"vertex_shader creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}

	CodeInfo fragment_code_info = get_code_info(frag_path);
	SDL_GPUShaderCreateInfo fragment_create_info{
		.code_size = fragment_code_info.size,
		.code = reinterpret_cast<uint8_t *>(fragment_code_info.buffer.data()),
		.entrypoint = "main",
		.format = SDL_GPU_SHADERFORMAT_SPIRV,
		.stage = SDL_GPU_SHADERSTAGE_FRAGMENT,
		.num_samplers = frag_infos.num_samplers,
		.num_storage_textures = frag_infos.num_storage_textures,
		.num_storage_buffers = frag_infos.num_storage_buffers,
		.num_uniform_buffers = frag_infos.num_uniform_buffers,
		.props = 0
	};
	fragment_shader = SDL_CreateGPUShader(this->device, &fragment_create_info);
	if (!fragment_shader) {
		throw std::runtime_error(
			"fragment_shader creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
}

Shader::~Shader() {
	if (fragment_shader)
		SDL_ReleaseGPUShader(device, fragment_shader);
	if (vertex_shader)
		SDL_ReleaseGPUShader(device, vertex_shader);
}

SDL_GPUShader *Shader::get_vertex() const {
	return vertex_shader;
}

SDL_GPUShader *Shader::get_fragment() const {
	return fragment_shader;
}

CodeInfo Shader::get_code_info(const std::string &code_path) {
	CodeInfo code_info;

	std::ifstream file(
		code_path, std::ios::ate | std::ios::binary
	);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file! \"" + code_path + "\"");
	}
	code_info.size = file.tellg();
	file.seekg(0);
	code_info.buffer.resize(code_info.size);
	file.read(code_info.buffer.data(), code_info.size);
	file.close();
	return code_info;
}

}  // namespace lili
