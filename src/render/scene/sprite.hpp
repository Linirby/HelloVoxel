/// Sprite scene object.
#pragma once

#include <SDL3/SDL_gpu.h>
#include <string>
#include <memory>

#include "render/renderer.hpp"
#include "render/core/texture.hpp"
#include "world/material.hpp"

namespace lili {

class Sprite {
public:
	/// Create a sprite.
	Sprite();
	/// Destroy the sprite.
	~Sprite() = default;

	/// Set the sprite texture from a file path.
	void set_texture(Renderer *renderer, const std::string &path);
	/// Set the sprite position.
	void set_position(Vec3 position);
	/// Set the sprite scale.
	void set_scale(Vec3 scale);
	/// Set the sprite rotation.
	void set_rotation(Vec3 rotation);
	/// Draw the sprite.
	void draw(Renderer *renderer);

private:
	std::unique_ptr<Texture> texture = nullptr;
	std::unique_ptr<Material> material = nullptr;
	std::unique_ptr<GPUMesh> mesh = nullptr;
	Model model;

	Vec3 position;
	Vec3 scale;
	Vec3 rotation;
};

}  // namespace lili
