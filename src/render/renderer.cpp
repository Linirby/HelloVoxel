#include <stdexcept>
#include "render/renderer.hpp"

namespace lili {

void Renderer::init(SDL_Window *window) {
	shared_window = window;
	int win_w, win_h;
	SDL_GetWindowSize(window, &win_w, &win_h);
	SDL_GPUTextureCreateInfo depth_info{
		.type = SDL_GPU_TEXTURETYPE_2D,
		.format = SDL_GPU_TEXTUREFORMAT_D16_UNORM,
		.usage = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET,
		.width = static_cast<uint32_t>(win_w),
		.height = static_cast<uint32_t>(win_h),
		.layer_count_or_depth = 1,
		.num_levels = 1,
		.sample_count = SDL_GPU_SAMPLECOUNT_1,
		.props = 0
	};
	depth_texture = SDL_CreateGPUTexture(device, &depth_info);
	if (!depth_texture) {
		throw std::runtime_error(
			"res.depth_texture creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
}

void Renderer::begin_frame() {
	cmd_buffer = SDL_AcquireGPUCommandBuffer(device);
	if (!cmd_buffer) {
		throw std::runtime_error("Failed to acquire command buffer!");
	}
	SDL_GPUTexture *swapchain_texture = nullptr;
	uint32_t width, height;
	bool success = SDL_WaitAndAcquireGPUSwapchainTexture(
		cmd_buffer,
		shared_window,
		&swapchain_texture,
		&width,
		&height
	);
	if (!success) {
		throw std::runtime_error("Failed to acquire swapchain texture!");
	}
	if (!swapchain_texture) {
		SDL_SubmitGPUCommandBuffer(cmd_buffer);
		return;
	}
	SDL_GPUColorTargetInfo color_target_info{
		.texture = swapchain_texture,
		.clear_color = SDL_FColor{ 0.1f, 0.1f, 0.1f, 1.0f },
		.load_op = SDL_GPU_LOADOP_CLEAR,
		.store_op = SDL_GPU_STOREOP_STORE,
	};
	SDL_GPUDepthStencilTargetInfo depth_target_info{
		.texture = depth_texture,
		.clear_depth = 1.0f,
		.load_op = SDL_GPU_LOADOP_CLEAR,
		.store_op = SDL_GPU_STOREOP_DONT_CARE,
		.stencil_load_op = SDL_GPU_LOADOP_DONT_CARE,
		.stencil_store_op = SDL_GPU_STOREOP_DONT_CARE,
		.cycle = false,
		.clear_stencil = 0
	};
	render_pass = SDL_BeginGPURenderPass(
		cmd_buffer,
		&color_target_info,
		1,
		&depth_target_info
	);
}

void Renderer::end_frame() {
	SDL_EndGPURenderPass(render_pass);
	SDL_SubmitGPUCommandBuffer(cmd_buffer);
}

void Renderer::draw_mesh(Mesh mesh, Camera camera) {
	if (!render_pass) {
		throw std::runtime_error("RENDERER: render_pass is not initialized!");
	}
	uint32_t mesh_index_count = static_cast<uint32_t>(mesh.indices.size());
	SDL_DrawGPUIndexedPrimitives(render_pass, mesh_index_count, 1, 0, 0, 0);
}

}
