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
	Renderer();
	~Renderer();

	void set_window(Window *window);
	void on_window_resized(int new_width, int new_height);

	SDL_GPUDevice *get_device() const;

	void set_directional_light(Vec3 direction, Vec4 color, Vec4 ambient);

	bool begin_frame(Camera camera);
	void submit(const Model &model, Mat4 transform, RenderLayer layer);
	void end_frame();

private:
	Window *window;
	SDL_GPUDevice *device;

	SDL_GPUTexture *depth_texture;

	SDL_GPUTexture *current_swapchain_texture;
	SDL_GPUCommandBuffer *current_cmd_buffer;

	Shader *world_shader;
	Shader *ui_shader;

	WorldPipeline *world_pipeline;
	UIPipeline *ui_pipeline;

	WorldPass *world_pass;
	UIPass *ui_pass;

	std::vector<DrawCommand> world_queue;
	std::vector<DrawCommand> ui_queue;

	Mat4 projection_view_3d;
	Mat4 projection_2d;
	DirectionalLightGPU directional_light;

	void init_device();
	void init_depth_texture();
	void init_shaders();
	void init_pipelines();
	void init_passes();
};

}  // namespace lili
