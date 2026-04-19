#ifndef RENDER_RENDERER_HPP
# define RENDER_RENDERER_HPP

# include <SDL3/SDL.h>

# include "render/camera.hpp"
# include "render/shader.hpp"
# include "render/model.hpp"

namespace lili {

enum class RenderLayer {
	World3D,
	UI2D
};

struct DrawCommand {
	const Model &model;
	Mat4 transform;
};

class Renderer {
public:
	Renderer(SDL_Window *window);
	~Renderer();

	// Holy trinity of rendering, The TRIFORCE!
	bool begin_frame(Camera camera);
	void submit(const Model &model, Mat4 transform, RenderLayer layer);
	void end_frame();

	SDL_GPUDevice *get_device() const;

private:
	SDL_Window *window;
	SDL_GPUDevice *device = nullptr;
	Shader *shader = nullptr;
	SDL_GPUGraphicsPipeline *world_pipeline = nullptr;
	SDL_GPUTexture *depth_texture = nullptr;
	SDL_GPUGraphicsPipeline *ui_pipeline = nullptr;

	SDL_GPUCommandBuffer *current_cmd_buffer = nullptr;
	SDL_GPURenderPass *current_render_pass = nullptr;

	std::vector<DrawCommand> world_queue;
	std::vector<DrawCommand> ui_queue;

	Mat4 projection_view_3d;
	Mat4 projection_2d;

	void init_device();
	void init_depth_texture();

	void init_world_pipeline();
	void init_ui_pipeline();
};

}  // namespace lili

#endif
