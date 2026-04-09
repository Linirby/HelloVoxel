#include <string>
#include <stdexcept>
#include "render/renderer.hpp"

namespace lili {

Renderer::Renderer(SDL_Window *window) {
	this->window = window;
	device = SDL_CreateGPUDevice(
		SDL_GPU_SHADERFORMAT_SPIRV, true, "vulkan"
	);
	if (!device) {
		throw std::runtime_error(
			"core.device creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
	if (!SDL_ClaimWindowForGPUDevice(device, this->window)) {
		throw std::runtime_error(
			"SDL_ClaimWindowForGPUDevice() failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
	chunk_graphics_pipeline = nullptr;
	depth_texture = nullptr;

	current_cmd_buffer = nullptr;
	current_render_pass = nullptr;
}

Renderer::~Renderer() {

}

void Renderer::begin_frame() {

}

void Renderer::draw_mesh(Mesh mesh, Camera camera) {

}

void Renderer::end_frame() {

}

SDL_GPUDevice *Renderer::get_device() const {
	return device;
}

}  // namespace lili
