#pragma once

#include <SDL3/SDL_gpu.h>
#include <vector>
#include <string>

namespace lili {

struct CodeInfo {
	size_t size;
	std::vector<char> buffer;
};

class Shader {
public:
	Shader(
		SDL_GPUDevice *device,
		const std::string &vert_path,
		const std::string &frag_path
	);
	~Shader();

	SDL_GPUShader *get_vertex() const;
	SDL_GPUShader *get_fragment() const;

private:
	SDL_GPUDevice *device = nullptr;
	SDL_GPUShader *vertex_shader = nullptr;
	SDL_GPUShader *fragment_shader = nullptr;

	CodeInfo get_code_info(const std::string &code_path);
};

}  // namespace lili