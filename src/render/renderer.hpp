#ifndef RENDER_RENDERER_HPP
# define RENDER_RENDERER_HPP

# include <SDL3/SDL.h>
# include "geometry/mesher.hpp"
# include "render/camera.hpp"

namespace lili {

class Renderer {
public:
	Renderer(SDL_Window *window);
	~Renderer();

	// Holy trinity of rendering, The TRIFORCE!
	void begin_frame();
	void draw_mesh(Mesh mesh, Camera camera);
	void end_frame();

	SDL_GPUDevice *get_device() const;

private:
	SDL_Window *window;
	SDL_GPUDevice *device = nullptr;
	SDL_GPUGraphicsPipeline *chunk_graphics_pipeline = nullptr;
	SDL_GPUTexture *depth_texture = nullptr;

	SDL_GPUCommandBuffer *current_cmd_buffer = nullptr;
	SDL_GPURenderPass *current_render_pass = nullptr;
};

}  // namespace lili

#endif
