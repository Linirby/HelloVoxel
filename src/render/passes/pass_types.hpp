#pragma once

#include "render/scene/model.hpp"
#include "geometry/mat4x4.hpp"

namespace lili {

enum class RenderLayer {
	World3D,
	UI2D
};

struct DrawCommand {
	const Model &model;
	Mat4 transform;
};

}  // namespace lili
