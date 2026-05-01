#ifndef RENDER_PASSES_RENDERER_HPP
# define RENDER_PASSES_RENDERER_HPP

# include <SDL3/SDL.h>

# include "core/window.hpp"

# include "render/core/shader.hpp"
# include "render/scene/camera.hpp"
# include "render/scene/model.hpp"

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
	Renderer(Window *window);
	~Renderer();

	// Holy trinity of rendering, The TRIFORCE!
	bool begin_frame(Camera camera);
	void submit(const Model &model, Mat4 transform, RenderLayer layer);
	void end_frame();

	SDL_GPUDevice *get_device() const;

private:
	SDL_Window *window = nullptr;
	SDL_GPUDevice *device = nullptr;
	Shader *world_shader = nullptr;
	Shader *ui_shader = nullptr;
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

#endif  // RENDER_PASSES_RENDERER_HPP
