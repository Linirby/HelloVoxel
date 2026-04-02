#include <iostream>
#include <fstream>
#include <cstring>
#include <SDL3_image/SDL_image.h>

#include "app.hpp"
#include "math/mat4x4.hpp"

namespace Utils {

CodeInfo get_code_info(std::string filename) {
	CodeInfo code_info;

	std::ifstream vert_file(
		filename, std::ios::ate | std::ios::binary
	);
	if (!vert_file.is_open()) {
		throw std::runtime_error("Failed to open file! \"" + filename + "\"");
	}
	code_info.size = vert_file.tellg();
	vert_file.seekg(0);
	code_info.buffer.resize(code_info.size);
	vert_file.read(code_info.buffer.data(), code_info.size);
	vert_file.close();
	return code_info;
}

void transfer_buffer_to_gpu(
	SDL_GPUDevice *device,
	void *data,
	SDL_GPUBuffer *buffer,
	uint32_t data_size
) {
	SDL_GPUTransferBufferCreateInfo transfer_buffer_info{
		.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
		.size = data_size,
		.props = 0
	};
	SDL_GPUTransferBuffer *transfer_buffer = SDL_CreateGPUTransferBuffer(
		device, &transfer_buffer_info
	);
	void *map = SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
	std::memcpy(map, data, data_size);
	SDL_UnmapGPUTransferBuffer(device, transfer_buffer);
	SDL_GPUCommandBuffer *upload_cmd = SDL_AcquireGPUCommandBuffer(device);
	SDL_GPUCopyPass *copy_pass = SDL_BeginGPUCopyPass(upload_cmd);
	SDL_GPUTransferBufferLocation src{
		.transfer_buffer = transfer_buffer,
		.offset = 0
	};
	SDL_GPUBufferRegion dst{
		.buffer = buffer,
		.offset = 0,
		.size = data_size
	};
	SDL_UploadToGPUBuffer(copy_pass, &src, &dst, false);
	SDL_EndGPUCopyPass(copy_pass);
	SDL_SubmitGPUCommandBuffer(upload_cmd);
	SDL_ReleaseGPUTransferBuffer(device, transfer_buffer);
}

void load_img_to_gpu_texture(
	SDL_GPUDevice *device, SDL_GPUTexture **texture, std::string filename
) {
	SDL_Surface *temp_surface = IMG_Load(filename.c_str());
	if (!temp_surface) {
		throw std::runtime_error("Failed to load image " + filename);
	}
	SDL_Surface *surface = SDL_ConvertSurface(
		temp_surface, SDL_PIXELFORMAT_ABGR8888
	);
	SDL_DestroySurface(temp_surface);
	SDL_GPUTextureCreateInfo texture_create_info{
		.type = SDL_GPU_TEXTURETYPE_2D,
		.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
		.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER,
		.width = static_cast<uint32_t>(surface->w),
		.height = static_cast<uint32_t>(surface->h),
		.layer_count_or_depth = 1,
		.num_levels = 1,
		.sample_count = SDL_GPU_SAMPLECOUNT_1,
		.props = 0
	};
	*texture = SDL_CreateGPUTexture(device, &texture_create_info);
	uint32_t image_size = surface->w * surface->h * 4;
	SDL_GPUTransferBufferCreateInfo transfer_buffer_info{
		.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
		.size = image_size,
		.props = 0
	};
	SDL_GPUTransferBuffer *transfer_buffer = SDL_CreateGPUTransferBuffer(
		device, &transfer_buffer_info
	);
	void *map = SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
	std::memcpy(map, surface->pixels, image_size);
	SDL_UnmapGPUTransferBuffer(device, transfer_buffer);
	SDL_GPUCommandBuffer *cmd = SDL_AcquireGPUCommandBuffer(device);
	SDL_GPUCopyPass *copy_pass = SDL_BeginGPUCopyPass(cmd);
	SDL_GPUTextureTransferInfo src{
		.transfer_buffer = transfer_buffer,
		.offset = 0
	};
	SDL_GPUTextureRegion dst{
		.texture = *texture,
		.w = static_cast<uint32_t>(surface->w),
		.h = static_cast<uint32_t>(surface->h),
		.d = 1
	};
	SDL_UploadToGPUTexture(copy_pass, &src, &dst, false);
	SDL_EndGPUCopyPass(copy_pass);
	SDL_SubmitGPUCommandBuffer(cmd);
	SDL_ReleaseGPUTransferBuffer(device, transfer_buffer);
	SDL_DestroySurface(surface);
}

} // namespace Utils

//
//	Public API
//

void App::run() {
	init_shapes();
	std::cout << "Shapes successfully initialized!" << '\n';
	init_window();
	std::cout << "Window successfully initialized!" << '\n';
	init_device();
	std::cout << "Device successfully initialized!" << '\n';
	init_textures();
	std::cout << "Textures successfully initialized!" << '\n';
	init_buffers();
	std::cout << "Buffers successfully initialized!" << '\n';
	init_shaders();
	std::cout << "Shaders successfully initialized!" << '\n';
	init_graphics_pipeline();
	std::cout << "Graphics pipeline successfully initialized!" << '\n';
	mainloop();
	cleanup();
}

//
//	Private API
//

void App::init_shapes() {
	atlas_voxel = { { -0.5f, 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f } };
	vox_rotation = 0.0f;
}

void App::init_window() {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		throw std::runtime_error("Failed to initialize SDL!");
	}
	window = SDL_CreateWindow(
		"HelloVoxel", WIN_WIDTH, WIN_HEIGHT, 0
	);
	if (!window) {
		throw std::runtime_error("Failed to create SDL_Window!");
	}
	is_running = true;
}

void App::init_device() {
	device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, "vulkan");
	if (!device) {
		throw std::runtime_error("Failed to create SDL_GPUDevice!");
	}
	if (!SDL_ClaimWindowForGPUDevice(device, window)) {
		throw std::runtime_error("Failed to claim window!");
	}
}

void App::init_textures() {
	SDL_GPUTextureCreateInfo depth_info{
		.type = SDL_GPU_TEXTURETYPE_2D,
		.format = SDL_GPU_TEXTUREFORMAT_D16_UNORM,
		.usage = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET,
		.width = WIN_WIDTH,
		.height = WIN_HEIGHT,
		.layer_count_or_depth = 1,
		.num_levels = 1,
		.sample_count = SDL_GPU_SAMPLECOUNT_1,
		.props = 0
	};
	depth_texture = SDL_CreateGPUTexture(device, &depth_info);
	if (!depth_texture) {
		throw std::runtime_error("Failed to create depth texture!");
	}
	Utils::load_img_to_gpu_texture(
		device, &atlas_voxel_texture, "assets/cube_atlas.png"
	);
	SDL_GPUSamplerCreateInfo sampler_info{
		.min_filter = SDL_GPU_FILTER_NEAREST,
		.mag_filter = SDL_GPU_FILTER_NEAREST,
		.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST,
		.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_REPEAT,
		.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_REPEAT,
		.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_REPEAT,
		.props = 0
	};
	texture_sampler = SDL_CreateGPUSampler(device, &sampler_info);
}

void App::init_buffers() {
	uint32_t vertices_buffer_size = sizeof(atlas_voxel.vertices);
	SDL_GPUBufferCreateInfo vertices_buffer_create_info = {
		.usage = SDL_GPU_BUFFERUSAGE_VERTEX,
		.size = vertices_buffer_size,
		.props = 0
	};
	vertex_buffer = SDL_CreateGPUBuffer(device, &vertices_buffer_create_info);
	Utils::transfer_buffer_to_gpu(
		device, atlas_voxel.vertices, vertex_buffer, vertices_buffer_size
	);
	uint32_t indices_buffer_size = sizeof(atlas_voxel.indices);
	SDL_GPUBufferCreateInfo indices_buffer_create_info = {
		.usage = SDL_GPU_BUFFERUSAGE_INDEX,
		.size = indices_buffer_size,
		.props = 0
	};
	index_buffer = SDL_CreateGPUBuffer(device, &indices_buffer_create_info);
	Utils::transfer_buffer_to_gpu(
		device, atlas_voxel.indices, index_buffer, indices_buffer_size
	);
}

void App::init_shaders() {
	CodeInfo vert_code_info = Utils::get_code_info(
		"shader/triangle.vert.spv"
	);
	SDL_GPUShaderCreateInfo vert_shader_create_info{
		.code_size = vert_code_info.size,
		.code = reinterpret_cast<uint8_t *>(vert_code_info.buffer.data()),
		.entrypoint = "main",
		.format = SDL_GPU_SHADERFORMAT_SPIRV,
		.stage = SDL_GPU_SHADERSTAGE_VERTEX,
		.num_samplers = 0,
		.num_storage_textures = 0,
		.num_storage_buffers = 0,
		.num_uniform_buffers = 1,
		.props = 0
	};
	vertex_shader = SDL_CreateGPUShader(device, &vert_shader_create_info);
	if (!vertex_shader) {
		throw std::runtime_error("Failed to create vertex shader!");
	}

	CodeInfo frag_code_info = Utils::get_code_info(
		"shader/triangle.frag.spv"
	);
	SDL_GPUShaderCreateInfo frag_shader_ci{
		.code_size = frag_code_info.size,
		.code = reinterpret_cast<uint8_t *>(frag_code_info.buffer.data()),
		.entrypoint = "main",
		.format = SDL_GPU_SHADERFORMAT_SPIRV,
		.stage = SDL_GPU_SHADERSTAGE_FRAGMENT,
		.num_samplers = 1,
		.num_storage_textures = 0,
		.num_storage_buffers = 0,
		.num_uniform_buffers = 0,
		.props = 0
	};
	fragment_shader = SDL_CreateGPUShader(device, &frag_shader_ci);
	if (!fragment_shader) {
		throw std::runtime_error("Failed to create fragment shader!");
	}
}

void App::init_graphics_pipeline() {
	SDL_GPUVertexBufferDescription vertex_buffer_desc{
		.slot = 0,
		.pitch = sizeof(float) * 5,
		.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
		.instance_step_rate = 0
	};
	SDL_GPUVertexAttribute vertex_attributes[2] = {
		(SDL_GPUVertexAttribute){
			.location = 0,
			.buffer_slot = 0,
			.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
			.offset = 0
		},
		(SDL_GPUVertexAttribute){
			.location = 1,
			.buffer_slot = 0,
			.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2,
			.offset = sizeof(float) * 3
		}
	};
	SDL_GPUColorTargetDescription color_target_desc{
		.format = SDL_GetGPUSwapchainTextureFormat(device, window),
		.blend_state = {
			.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_COLOR,
			.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_DST_COLOR,
			.color_blend_op = SDL_GPU_BLENDOP_ADD,
			.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_COLOR,
			.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_DST_COLOR,
			.alpha_blend_op = SDL_GPU_BLENDOP_ADD,
			.color_write_mask = SDL_GPU_COLORCOMPONENT_R |
				SDL_GPU_COLORCOMPONENT_G |
				SDL_GPU_COLORCOMPONENT_B |
				SDL_GPU_COLORCOMPONENT_A,
			.enable_blend = false,
			.enable_color_write_mask = true,
		}
	};
	SDL_GPUGraphicsPipelineCreateInfo create_info{
		.vertex_shader = vertex_shader,
		.fragment_shader = fragment_shader,
		.vertex_input_state = {
			.vertex_buffer_descriptions = &vertex_buffer_desc,
			.num_vertex_buffers = 1,
			.vertex_attributes = vertex_attributes,
			.num_vertex_attributes = 2
		},
		.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
		.rasterizer_state = {
			.fill_mode = SDL_GPU_FILLMODE_FILL,
			.cull_mode = SDL_GPU_CULLMODE_BACK,
			.front_face = SDL_GPU_FRONTFACE_CLOCKWISE,
		},
		.multisample_state = {
			.sample_count = SDL_GPU_SAMPLECOUNT_1,
		},
		.depth_stencil_state = {
			.compare_op = SDL_GPU_COMPAREOP_GREATER,
			.back_stencil_state = { SDL_GPU_STENCILOP_ZERO },
			.front_stencil_state = { SDL_GPU_STENCILOP_ZERO },
			.compare_mask = 0,
			.write_mask = 0,
			.enable_depth_test = true,
			.enable_depth_write = true,
			.enable_stencil_test = false
		},
		.target_info = {
			.color_target_descriptions = &color_target_desc,
			.num_color_targets = 1,
			.depth_stencil_format = SDL_GPU_TEXTUREFORMAT_D16_UNORM,
			.has_depth_stencil_target = true
		},
		.props = 0
	};
	pipeline = SDL_CreateGPUGraphicsPipeline(device, &create_info);
}

void App::handle_events() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) {
			is_running = false;
		}
		if (event.type == SDL_EVENT_KEY_DOWN) {
			if (event.key.key == SDLK_ESCAPE) {
				is_running = false;
			}
		}
	}
}

void App::update(float dt) {
	vox_rotation += 45.0f * dt;
	if (vox_rotation >= 360.0f || vox_rotation < 0) {
		vox_rotation = 0;
	}
}

void App::render() {
	lili::Mat4 translation = lili::Mat4::translate({ 0.0f, 0.0f, 0.0f });
	lili::Mat4 rotation_y = lili::Mat4::rotation_y(
		(float)vox_rotation*3.14f/180.0f
	);
	lili::Mat4 model = translation * rotation_y;
	lili::Mat4 view = lili::Mat4::look_at(
		{ 0.0f, 1.0f, 3.0f },  // eye
		{ 0.0f, 0.0f, 0.0f },  // center
		{ 0.0f, 1.0f, 0.0f }   // up
	);
	lili::Mat4 proj = lili::Mat4::perspective(
		70.0f*3.14f/180.0f,                    // FOV Y (in rad)
		(float)WIN_WIDTH / (float)WIN_HEIGHT,  // aspect ratio
		0.1f,                                  // near distance unit
		100.0f                                 // far distance unit
	);
	lili::Mat4 mvp = proj * view * model;
	SDL_GPUCommandBuffer *cmd_buffer = SDL_AcquireGPUCommandBuffer(device);
	if (!cmd_buffer) {
		throw std::runtime_error("Failed to acquire command buffer!");
	}
	SDL_GPUTexture *swapchain_texture = nullptr;
	uint32_t width, height;
	bool res = SDL_WaitAndAcquireGPUSwapchainTexture(
		cmd_buffer,
		window,
		&swapchain_texture,
		&width,
		&height
	);
	if (!res) {
		throw std::runtime_error("Failed to acquire swapchain texture!");
	}
	if (!swapchain_texture) {
		SDL_SubmitGPUCommandBuffer(cmd_buffer);
		return;
	}
	SDL_PushGPUVertexUniformData(cmd_buffer, 0, &mvp, sizeof(lili::Mat4));
	SDL_GPUColorTargetInfo color_target_info{
		.texture = swapchain_texture,
		.clear_color = SDL_FColor{ 0.1f, 0.1f, 0.1f, 1.0f },
		.load_op = SDL_GPU_LOADOP_CLEAR,
		.store_op = SDL_GPU_STOREOP_STORE,
	};
	SDL_GPUDepthStencilTargetInfo depth_target_info{
		.texture = depth_texture,
		.clear_depth = 0.0f,
		.load_op = SDL_GPU_LOADOP_CLEAR,
		.store_op = SDL_GPU_STOREOP_DONT_CARE,
		.stencil_load_op = SDL_GPU_LOADOP_DONT_CARE,
		.stencil_store_op = SDL_GPU_STOREOP_DONT_CARE,
		.cycle = false,
		.clear_stencil = 0
	};
	SDL_GPURenderPass *render_pass = SDL_BeginGPURenderPass(
		cmd_buffer,
		&color_target_info,
		1,
		&depth_target_info
	);
	SDL_BindGPUGraphicsPipeline(render_pass, pipeline);
	SDL_GPUBufferBinding vertex_binding{
		.buffer = vertex_buffer,
		.offset = 0
	};
	SDL_BindGPUVertexBuffers(render_pass, 0, &vertex_binding, 1);
	SDL_GPUBufferBinding index_binding{
		.buffer = index_buffer,
		.offset = 0
	};
	SDL_BindGPUIndexBuffer(
		render_pass, &index_binding, SDL_GPU_INDEXELEMENTSIZE_16BIT
	);
	SDL_GPUTextureSamplerBinding atlas_texture_sampler_binding{
		.texture = atlas_voxel_texture,
		.sampler = texture_sampler
	};
	SDL_BindGPUFragmentSamplers(
		render_pass, 0, &atlas_texture_sampler_binding, 1
	);
	SDL_DrawGPUIndexedPrimitives(render_pass, 36, 1, 0, 0, 0);
	SDL_EndGPURenderPass(render_pass);
	SDL_SubmitGPUCommandBuffer(cmd_buffer);
}

void App::mainloop() {
	Uint64 last = SDL_GetTicks();

	while (is_running) {
		Uint64 now = SDL_GetTicks();
		float dt = (now - last) / 1000.0f;
		last = now;
		
		handle_events();
		update(dt);
		render();
	}
}

void App::cleanup() {
	if (pipeline) SDL_ReleaseGPUGraphicsPipeline(device, pipeline);
	if (fragment_shader) SDL_ReleaseGPUShader(device, fragment_shader);
	if (vertex_shader) SDL_ReleaseGPUShader(device, vertex_shader);
	if (index_buffer) SDL_ReleaseGPUBuffer(device, index_buffer);
	if (vertex_buffer) SDL_ReleaseGPUBuffer(device, vertex_buffer);
	if (texture_sampler) SDL_ReleaseGPUSampler(device, texture_sampler);
	if (atlas_voxel_texture) SDL_ReleaseGPUTexture(device, atlas_voxel_texture);
	if (depth_texture) SDL_ReleaseGPUTexture(device, depth_texture);
	if (device) SDL_DestroyGPUDevice(device);
	if (window) SDL_DestroyWindow(window);
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_Quit();
}
