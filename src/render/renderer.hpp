#ifndef RENDER_RENDERER_HPP
# define RENDER_RENDERER_HPP

# include <SDL3/SDL.h>

# include "render/camera.hpp"
# include "render/shader.hpp"
# include "render/buffer.hpp"
# include "render/texture.hpp"

# include "geometry/chunk.hpp"

namespace lili {

class Renderer {
public:
	Renderer(SDL_Window *window);
	~Renderer();

	// Holy trinity of rendering, The TRIFORCE!
	bool begin_frame(Camera camera);
	void draw_chunk();
	void end_frame();

	SDL_GPUDevice *get_device() const;

private:
	SDL_Window *window;
	SDL_GPUDevice *device = nullptr;
	Shader *shader = nullptr;
	Buffer *buffer = nullptr;
	SDL_GPUGraphicsPipeline *graphics_pipeline = nullptr;
	SDL_GPUTexture *depth_texture = nullptr;

	SDL_GPUCommandBuffer *current_cmd_buffer = nullptr;
	SDL_GPURenderPass *current_render_pass = nullptr;

	Chunk test_chunk;
	Texture *voxel_texture;

	void init_device();
	void init_depth_texture();
	void init_chunk();
	void init_graphics_pipeline();
};

}  // namespace lili

#endif
