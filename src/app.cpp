#include <iostream>

#include "app.hpp"
#include "utils.hpp"
#include "math/mat4x4.hpp"
#include "math/utils.hpp"
#include "geometry/mesher.hpp"
#include "geometry/block.hpp"

void App::run() {
	init_window();
	init_device();
	init_textures();
	init_test_chunk();
	init_buffers();
	init_shaders();
	init_graphics_pipeline();
	mainloop();
	cleanup();
}

void App::init_window() {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		throw std::runtime_error(
			"SDL_Init() failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
	core.window = SDL_CreateWindow(
		"HelloVoxel", WIN_WIDTH, WIN_HEIGHT, 0
	);
	if (!core.window) {
		throw std::runtime_error(
			"core.window creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
	SDL_SetWindowRelativeMouseMode(core.window, true);
	is_running = true;
	std::cout << "core.window successfully initialized!" << '\n';
}

void App::init_device() {
	core.device = SDL_CreateGPUDevice(
		SDL_GPU_SHADERFORMAT_SPIRV, true, "vulkan"
	);
	if (!core.device) {
		throw std::runtime_error(
			"core.device creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
	if (!SDL_ClaimWindowForGPUDevice(core.device, core.window)) {
		throw std::runtime_error(
			"SDL_ClaimWindowForGPUDevice() failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
	std::cout << "core.device successfully initialized!" << '\n';
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
	res.depth_texture = SDL_CreateGPUTexture(core.device, &depth_info);
	if (!res.depth_texture) {
		throw std::runtime_error(
			"res.depth_texture creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
	std::cout << "res.depth_texture successfully created!" << '\n';
	Utils::load_img_to_gpu_texture(
		core.device, &res.atlas_voxel_texture, "assets/cube_atlas.png"
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
	res.texture_sampler = SDL_CreateGPUSampler(core.device, &sampler_info);
	if (!res.texture_sampler) {
		throw std::runtime_error(
			"res.texture_sampler creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
	std::cout << "res.texture_sampler successfully created!" << '\n';
}

void App::init_test_chunk() {
	for (int x = 0; x < lili::Chunk::SIZE; ++x) {
		for (int z = 0; z < lili::Chunk::SIZE; ++z) {
			test_chunk.set_block(lili::BLOCK_ID_DEBUG, x, 0, z);
		}
	}
	for (int y = 0; y < lili::Chunk::SIZE; ++y) {
		test_chunk.set_block(lili::BLOCK_ID_DEBUG, 0,  y, 0);
		test_chunk.set_block(lili::BLOCK_ID_DEBUG, 0,  y, 15);
		test_chunk.set_block(lili::BLOCK_ID_DEBUG, 15, y, 0);
		test_chunk.set_block(lili::BLOCK_ID_DEBUG, 15, y, 15);
	}
	for (int x = 0; x < lili::Chunk::SIZE; ++x) {
		for (int z = 0; z < lili::Chunk::SIZE; ++z) {
			test_chunk.set_block(lili::BLOCK_ID_DEBUG, x, 15, z);
		}
	}
	std::cout << "Chunk successfully initialized!" << '\n';
}

void App::init_buffers() {
	lili::Mesh chunk_mesh = lili::ChunkMesher::generate_mesh(test_chunk);
	chunk_index_count = static_cast<uint32_t>(chunk_mesh.indices.size());

	uint32_t vertices_buffer_size = (
		chunk_mesh.vertices.size() * sizeof(lili::Vertex)
	);
	SDL_GPUBufferCreateInfo vertices_buffer_create_info{
		.usage = SDL_GPU_BUFFERUSAGE_VERTEX,
		.size = vertices_buffer_size,
		.props = 0
	};
	res.vertex_buffer = SDL_CreateGPUBuffer(
		core.device, &vertices_buffer_create_info
	);
	if (!res.vertex_buffer) {
		throw std::runtime_error(
			"res.vertex_buffer creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
	std::cout << "Transfering vertices to gpu" << '\n';
	Utils::transfer_buffer_to_gpu(
		core.device,
		chunk_mesh.vertices.data(),
		res.vertex_buffer,
		vertices_buffer_size
	);

	uint32_t indices_buffer_size = chunk_mesh.indices.size() * sizeof(uint16_t);
	SDL_GPUBufferCreateInfo indices_buffer_create_info{
		.usage = SDL_GPU_BUFFERUSAGE_INDEX,
		.size = indices_buffer_size,
		.props = 0
	};
	res.index_buffer = SDL_CreateGPUBuffer(
		core.device, &indices_buffer_create_info
	);
	if (!res.index_buffer) {
		throw std::runtime_error(
			"res.index_buffer creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
	std::cout << "Transfering indices to gpu" << '\n';
	Utils::transfer_buffer_to_gpu(
		core.device,
		chunk_mesh.indices.data(),
		res.index_buffer,
		indices_buffer_size
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
	pipe.vertex_shader = SDL_CreateGPUShader(core.device, &vert_shader_create_info);
	if (!pipe.vertex_shader) {
		throw std::runtime_error(
			"pipe.vertex_shader creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
	std::cout << "pipe.vertex_shader successfully created!" << '\n';

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
	pipe.fragment_shader = SDL_CreateGPUShader(core.device, &frag_shader_ci);
	if (!pipe.fragment_shader) {
		throw std::runtime_error(
			"pipe.fragment_shader creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
	std::cout << "pipe.fragment_shader successfully created!" << '\n';
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
		.format = SDL_GetGPUSwapchainTextureFormat(core.device, core.window),
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
		.vertex_shader = pipe.vertex_shader,
		.fragment_shader = pipe.fragment_shader,
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
			.compare_op = SDL_GPU_COMPAREOP_LESS_OR_EQUAL,
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
	pipe.graphics_pipeline = SDL_CreateGPUGraphicsPipeline(core.device, &create_info);
	if (!pipe.graphics_pipeline) {
		throw std::runtime_error(
			"pipe.graphics_pipeline creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
	std::cout << "pipe.graphics_pipeline successfully initialized!" << '\n';
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
		if (event.type == SDL_EVENT_MOUSE_MOTION) {
			camera.process_mouse(event.motion.xrel, event.motion.yrel);
		}
	}
}

void App::update(float dt) {
	const bool *keys = SDL_GetKeyboardState(NULL);

	player.process_keyboard(keys, camera.front, camera.right, camera.up, dt);
	camera.position = player.position;
}

void App::render() {
	lili::Mat4 center_offset = lili::Mat4::translate({ 0.0f, 0.0f, 0.0f });
	lili::Mat4 rotation_y = lili::Mat4::rotation_y(lili::deg_to_rad(rotation));
	lili::Mat4 model = rotation_y * center_offset;

	lili::Mat4 view = camera.get_view_matrix();
	lili::Mat4 proj = lili::Mat4::perspective(
		lili::deg_to_rad(70.0f),                     // FOV Y (in rad)
		(float)WIN_WIDTH / (float)WIN_HEIGHT,  // aspect ratio
		0.1f,                                  // near distance unit
		100.0f                                 // far distance unit
	);
	lili::Mat4 mvp = proj * view * model;
	SDL_GPUCommandBuffer *cmd_buffer = SDL_AcquireGPUCommandBuffer(core.device);
	if (!cmd_buffer) {
		throw std::runtime_error("Failed to acquire command buffer!");
	}
	SDL_GPUTexture *swapchain_texture = nullptr;
	uint32_t width, height;
	bool success = SDL_WaitAndAcquireGPUSwapchainTexture(
		cmd_buffer,
		core.window,
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
	SDL_PushGPUVertexUniformData(cmd_buffer, 0, &mvp, sizeof(lili::Mat4));
	SDL_GPUColorTargetInfo color_target_info{
		.texture = swapchain_texture,
		.clear_color = SDL_FColor{ 0.1f, 0.1f, 0.1f, 1.0f },
		.load_op = SDL_GPU_LOADOP_CLEAR,
		.store_op = SDL_GPU_STOREOP_STORE,
	};
	SDL_GPUDepthStencilTargetInfo depth_target_info{
		.texture = res.depth_texture,
		.clear_depth = 1.0f,
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
	SDL_BindGPUGraphicsPipeline(render_pass, pipe.graphics_pipeline);
	SDL_GPUBufferBinding vertex_binding{
		.buffer = res.vertex_buffer,
		.offset = 0
	};
	SDL_BindGPUVertexBuffers(render_pass, 0, &vertex_binding, 1);
	SDL_GPUBufferBinding index_binding{
		.buffer = res.index_buffer,
		.offset = 0
	};
	SDL_BindGPUIndexBuffer(
		render_pass, &index_binding, SDL_GPU_INDEXELEMENTSIZE_16BIT
	);
	SDL_GPUTextureSamplerBinding atlas_texture_sampler_binding{
		.texture = res.atlas_voxel_texture,
		.sampler = res.texture_sampler
	};
	SDL_BindGPUFragmentSamplers(
		render_pass, 0, &atlas_texture_sampler_binding, 1
	);
	SDL_DrawGPUIndexedPrimitives(render_pass, chunk_index_count, 1, 0, 0, 0);
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
	if (pipe.graphics_pipeline)
		SDL_ReleaseGPUGraphicsPipeline(core.device, pipe.graphics_pipeline);
	if (pipe.fragment_shader)
		SDL_ReleaseGPUShader(core.device, pipe.fragment_shader);
	if (pipe.vertex_shader)
		SDL_ReleaseGPUShader(core.device, pipe.vertex_shader);
	std::cout << "Successfully cleaned pipeline" << '\n';
	if (res.index_buffer)
		SDL_ReleaseGPUBuffer(core.device, res.index_buffer);
	if (res.vertex_buffer)
		SDL_ReleaseGPUBuffer(core.device, res.vertex_buffer);
	if (res.texture_sampler)
		SDL_ReleaseGPUSampler(core.device, res.texture_sampler);
	if (res.atlas_voxel_texture)
		SDL_ReleaseGPUTexture(core.device, res.atlas_voxel_texture);
	if (res.depth_texture)
		SDL_ReleaseGPUTexture(core.device, res.depth_texture);
	std::cout << "Successfully cleaned ressources" << '\n';
	if (core.device)
		SDL_DestroyGPUDevice(core.device);
	if (core.window)
		SDL_DestroyWindow(core.window);
	std::cout << "Successfully cleaned core" << '\n';
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_Quit();
	std::cout << "Successfully cleaned SDL" << '\n';
}
