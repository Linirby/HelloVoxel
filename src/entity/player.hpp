#ifndef ENTITY_PLAYER_HPP
# define ENTITY_PLAYER_HPP

# include "math/vec3.hpp"

namespace lili {

class Player {
public:
	Vec3 position = { 8.0f, 18.0f, 8.0f };

	float fly_speed = 20.0f;

	void process_keyboard(
		const bool *keys,
		const Vec3 &cam_front,
		const Vec3 &cam_right,
		const Vec3 &cam_up,
		float dt
	);
};

}  // namespace lili

#endif
