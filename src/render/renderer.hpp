#ifndef RENDER_RENDERER_HPP
# define RENDER_RENDERER_HPP

# include <SDL3/SDL.h>
# include "geometry/mesher.hpp"
# include "render/camera.hpp"

namespace lili {

class Renderer {
public:
	void init(SDL_Window *window);

	void begin_frame();
	void end_frame();

	void draw_mesh(Mesh mesh, Camera camera);

private:
	SDL_Window *shared_window = nullptr;
	SDL_GPUDevice *device = nullptr;

	SDL_GPUCommandBuffer *cmd_buffer = nullptr;
	SDL_GPUTexture *depth_texture = nullptr;

	SDL_GPURenderPass *render_pass = nullptr;
};

}  // namespace lili

#endif
