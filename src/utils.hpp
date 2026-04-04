#ifndef UTILS_HPP
# define UTILS_HPP

# include <vector>
# include <string>

# include <SDL3/SDL_gpu.h>

struct CodeInfo {
	size_t size;
	std::vector<char> buffer;
};

namespace Utils {

CodeInfo get_code_info(std::string filename);

void transfer_buffer_to_gpu(
	SDL_GPUDevice *device,
	void *data,
	SDL_GPUBuffer *buffer,
	uint32_t buffer_size
);

void load_img_to_gpu_texture(
	SDL_GPUDevice *device, SDL_GPUTexture **texture, std::string filename
);

};

#endif
