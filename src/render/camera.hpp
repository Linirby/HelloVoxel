#ifndef RENDER_CAMERA_HPP
# define RENDER_CAMERA_HPP

# include "math/vec3.hpp"
# include "math/mat4x4.hpp"

namespace lili {

class Camera {
public:
	Vec3 position = { 0.0f, 0.0f, 0.0f };
	Vec3 front = { 0.0f, 0.0f, -1.0f };
	Vec3 right = { 1.0f, 0.0f,  0.0f };
	Vec3 up    = { 0.0f, 1.0f,  0.0f };

	float yaw = -90.0f;
	float pitch = 0.0f;
	float mouse_sensitivity = 0.1f;

	Camera(float yaw = -90.0f, float pitch = 0.0f);

	void process_mouse(float offset_x, float offset_y);
	Mat4 get_view_matrix() const;

private:
	void update_vectors();
};

}  // namespace lili

#endif
