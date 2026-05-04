#pragma once

#include "geometry/vec4.hpp"
#include "render/core/texture.hpp"

namespace lili {

struct MaterialProps {
	Vec4 color_tint = { 1.0f, 1.0f, 1.0f, 1.0f };
	float roughness = 1.0f;
	float metallic = 0.0f;
	float emission = 0.0f;
	float padding = 0.0f;
};

struct Material {
	MaterialProps properties;
	Texture *albedo_map = nullptr;

	Material() = default;
	Material(Texture *texture);
};

}  // namespace lili
