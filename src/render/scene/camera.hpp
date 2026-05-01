#ifndef RENDER_SCENE_CAMERA_HPP
# define RENDER_SCENE_CAMERA_HPP

# include "geometry/vec3.hpp"
# include "geometry/mat4x4.hpp"

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

	float fov_y = 90.0f;
	float near_dist = 0.3f;
	float far_dist = 100.0f;

	Camera(float yaw = -90.0f, float pitch = 0.0f, float fov_y = 90.0f);

	void set_rotation(float new_yaw, float new_pitch);
	void process_mouse(float offset_x, float offset_y);
	Mat4 get_view_matrix() const;

private:
	void update_vectors();
};

}  // namespace lili

#endif  // RENDER_SCENE_CAMERA_HPP
