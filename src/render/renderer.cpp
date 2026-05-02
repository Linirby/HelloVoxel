#include <string>
#include <stdexcept>

#include "render/renderer.hpp"
#include "render/scene/model.hpp"

#include "geometry/mat4x4.hpp"
#include "geometry/utils.hpp"

namespace lili {

Renderer::Renderer(Window *window) {
	this->window = window;

	init_device();
	init_depth_texture();
	init_shaders();
	init_pipelines();
	init_passes();
}

Renderer::~Renderer() {
	SDL_WaitForGPUIdle(device);

	if (ui_pass) delete ui_pass;
	if (world_pass) delete world_pass;
	if (ui_pipeline) delete ui_pipeline;
	if (world_pipeline) delete world_pipeline;
	if (ui_shader) delete ui_shader;
	if (world_shader) delete world_shader;
	if (depth_texture) SDL_ReleaseGPUTexture(device, depth_texture);
	if (device) SDL_DestroyGPUDevice(device);
}

bool Renderer::begin_frame(Camera camera) {
	current_cmd_buffer = SDL_AcquireGPUCommandBuffer(device);
	if (!current_cmd_buffer)
		throw std::runtime_error("Failed to acquire command buffer!");
	SDL_GPUTexture *swapchain_texture = nullptr;
	uint32_t width = 0;
	uint32_t height = 0;
	bool success = SDL_WaitAndAcquireGPUSwapchainTexture(
		current_cmd_buffer,
		window->get_sdl_window(),
		&swapchain_texture,
		&width,
		&height
	);
	if (!success)
		throw std::runtime_error("Failed to acquire swapchain texture!");
	if (!swapchain_texture) {
		SDL_SubmitGPUCommandBuffer(current_cmd_buffer);
		current_cmd_buffer = nullptr;
		current_render_pass = nullptr;
		return false;
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
	current_render_pass = SDL_BeginGPURenderPass(
		current_cmd_buffer,
		&color_target_info,
		1,
		&depth_target_info
	);

	int win_w, win_h;
	SDL_GetWindowSize(window->get_sdl_window(), &win_w, &win_h);
	float aspect = static_cast<float>(win_w) / static_cast<float>(win_h);

	Mat4 view = camera.get_view_matrix();
	Mat4 proj = Mat4::perspective(
		deg_to_rad(camera.fov_y),
		(float)win_w / (float)win_h,
		camera.near_dist,
		camera.far_dist
	);
	projection_view_3d = proj * view;
	projection_2d = Mat4::orthographic(
		0.0f, static_cast<float>(win_w),
		static_cast<float>(win_h), 0.0f,
		-1.0f, 1.0f
	);
	return true;
}

void Renderer::submit(const Model &model, Mat4 transform, RenderLayer layer) {
	if (layer == RenderLayer::World3D) {
		world_queue.push_back({ model, transform });
	} else if (layer == RenderLayer::UI2D) {
		ui_queue.push_back({ model, transform });
	}
}

void Renderer::end_frame() {
	world_pass->render(
		current_render_pass, current_cmd_buffer, projection_view_3d, world_queue
	);
	ui_pass->render(
		current_render_pass, current_cmd_buffer, projection_2d, ui_queue
	);

	world_queue.clear();
	ui_queue.clear();
	SDL_EndGPURenderPass(current_render_pass);
	SDL_SubmitGPUCommandBuffer(current_cmd_buffer);
}

SDL_GPUDevice *Renderer::get_device() const {
	return device;
}

void Renderer::init_device() {
	device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, nullptr);
	if (!device) {
		throw std::runtime_error(
			"Device creation failed!\n-> " + std::string(SDL_GetError())
		);
	}
	if (!SDL_ClaimWindowForGPUDevice(device, window->get_sdl_window())) {
		throw std::runtime_error(
			"SDL_ClaimWindowForGPUDevice() failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
	if (!SDL_SetGPUSwapchainParameters(
		device,
		window->get_sdl_window(),
		SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
		SDL_GPU_PRESENTMODE_MAILBOX
	)) {
		SDL_SetGPUSwapchainParameters(
			device,
			window->get_sdl_window(),
			SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
			SDL_GPU_PRESENTMODE_IMMEDIATE
		);
	}
}

void Renderer::init_depth_texture() {
	int win_w, win_h;
	SDL_GetWindowSize(window->get_sdl_window(), &win_w, &win_h);
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
			"Depth texture creation failed!\n-> " + std::string(SDL_GetError())
		);
	}
}

void Renderer::init_shaders() {
	world_shader = new Shader(
		device, "shader/world.vert.spv", "shader/world.frag.spv"
	);
	ui_shader = new Shader(
		device, "shader/ui.vert.spv", "shader/ui.frag.spv"
	);
}

void Renderer::init_pipelines() {
	if (world_shader) {
		world_pipeline = new WorldPipeline(
			device, window->get_sdl_window(), world_shader
		);
	}
	if (ui_shader) {
		ui_pipeline = new UIPipeline(
			device, window->get_sdl_window(), ui_shader
		);
	}
}

void Renderer::init_passes() {
	world_pass = new WorldPass(
		device, world_pipeline->get_sdl_pipeline(), world_shader
	);
	ui_pass = new UIPass(
		device, ui_pipeline->get_sdl_pipeline(), ui_shader
	);
}

}  // namespace lili
