#pragma once

#include <SDL3/SDL.h>

#include "core/window.hpp"

#include "render/core/shader.hpp"

#include "render/scene/camera.hpp"
#include "render/scene/model.hpp"

#include "render/passes/pass_types.hpp"
#include "render/passes/world_pass.hpp"
#include "render/passes/ui_pass.hpp"

#include "render/pipelines/world_pipeline.hpp"
#include "render/pipelines/ui_pipeline.hpp"

namespace lili {

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
	Window *window = nullptr;
	SDL_GPUDevice *device = nullptr;

	SDL_GPUTexture *depth_texture = nullptr;

	Shader *world_shader = nullptr;
	Shader *ui_shader = nullptr;

	WorldPipeline *world_pipeline = nullptr;
	UIPipeline *ui_pipeline = nullptr;

	SDL_GPUCommandBuffer *current_cmd_buffer = nullptr;
	SDL_GPURenderPass *current_render_pass = nullptr;

	WorldPass *world_pass = nullptr;
	UIPass *ui_pass = nullptr;

	std::vector<DrawCommand> world_queue;
	std::vector<DrawCommand> ui_queue;

	Mat4 projection_view_3d;
	Mat4 projection_2d;

	void init_device();
	void init_depth_texture();
	void init_shaders();
	void init_pipelines();
	void init_passes();
};

}  // namespace lili