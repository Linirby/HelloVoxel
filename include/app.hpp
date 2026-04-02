#ifndef APP_HPP
# define APP_HPP

# include <string>
# include <vector>
# include <SDL3/SDL.h>

# include "geometry/voxel.hpp"

const int WIN_WIDTH = 1280;
const int WIN_HEIGHT = 720;

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

class App {
public:
	void run();

private:
	lili::Voxel voxel;
	lili::AtlasVoxel atlas_voxel;
	float vox_rotation;

	SDL_Window *window = nullptr;
	SDL_GPUDevice *device = nullptr;
	SDL_GPUTexture *depth_texture = nullptr;
	SDL_GPUTexture *voxel_texture = nullptr;
	SDL_GPUTexture *atlas_voxel_texture = nullptr;
	SDL_GPUSampler *texture_sampler = nullptr;
	SDL_GPUBuffer *vertex_buffer = nullptr;
	SDL_GPUBuffer *index_buffer = nullptr;
	SDL_GPUShader *vertex_shader = nullptr;
	SDL_GPUShader *fragment_shader = nullptr;
	SDL_GPUGraphicsPipeline *pipeline = nullptr;

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
