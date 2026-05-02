#pragma once

#include <SDL3/SDL_gpu.h>

namespace lili {

class Shader;

class UIPipeline {
public:
	UIPipeline(SDL_GPUDevice *device, SDL_Window *window, Shader *shader);
	~UIPipeline();

	SDL_GPUGraphicsPipeline *get_sdl_pipeline();

private:
	SDL_GPUDevice *device = nullptr;
	SDL_Window *window = nullptr;
	Shader *shader = nullptr;

	SDL_GPUGraphicsPipeline *pipeline = nullptr;
};

}  // namespace lili