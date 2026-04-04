#ifndef APP_HPP
# define APP_HPP

# include <SDL3/SDL.h>
# include "geometry/voxel.hpp"


class App {
public:
	void run();

private:
	static constexpr int WIN_WIDTH = 1280;
	static constexpr int WIN_HEIGHT = 720;
	lili::AtlasVoxel atlas_voxel;
	float vox_rotation;

	struct Core {
		SDL_Window *window = nullptr;
		SDL_GPUDevice *device = nullptr;
	} core;
	struct Resources {
		SDL_GPUTexture *depth_texture = nullptr;
		SDL_GPUTexture *atlas_voxel_texture = nullptr;
		SDL_GPUSampler *texture_sampler = nullptr;
		SDL_GPUBuffer *vertex_buffer = nullptr;
		SDL_GPUBuffer *index_buffer = nullptr;
	} res;
	struct Pipeline {
		SDL_GPUShader *vertex_shader = nullptr;
		SDL_GPUShader *fragment_shader = nullptr;
		SDL_GPUGraphicsPipeline *graphics_pipeline = nullptr;
	} pipe;

	bool is_running = false;

	void init_shapes();
	void init_window();
	void init_device();
	void init_textures();
	void init_buffers();
	void init_shaders();
	void init_graphics_pipeline();

	void handle_events();
	void update(float dt);
	void render();
	
	void mainloop();
	void cleanup();
};

#endif
