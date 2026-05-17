#pragma once

#include <SDL3/SDL_gpu.h>

namespace lili {

class Shader;

class ShadowPipeline {
public:
	ShadowPipeline(SDL_GPUDevice *device, SDL_Window *window, Shader *shader);
	~ShadowPipeline();

	SDL_GPUGraphicsPipeline *get_sdl_pipeline();

private:
	SDL_GPUDevice *device = nullptr;
	SDL_Window *window = nullptr;
	Shader *shader = nullptr;

	SDL_GPUGraphicsPipeline *pipeline = nullptr;
};

}  // namespace lili
