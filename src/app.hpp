#ifndef APP_HPP
# define APP_HPP

# include <SDL3/SDL.h>
# include "geometry/chunk.hpp"
# include "render/camera.hpp"
# include "geometry/player.hpp"

class App {
public:
	void run();

private:
	static constexpr int WIN_WIDTH = 1280;
	static constexpr int WIN_HEIGHT = 720;
	float rotation = 0.0f;

	lili::Chunk test_chunk;	
	uint32_t chunk_index_count = 0;
	lili::Camera camera;
	lili::Player player;

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

	void init_window();
	void init_device();
	void init_textures();
	void init_test_chunk();
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
