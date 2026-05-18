#version 450

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_uv;
layout(location = 3) in float in_material_id;
layout(location = 4) in float in_ao;

layout(location = 0) out vec2 v_uv;
layout(location = 1) flat out uint v_material_id;
layout(location = 2) out float v_ao;

layout(set = 1, binding = 0) uniform CameraMVP {
	mat4 matrix;
} u_camera;

void main() {
	gl_Position = u_camera.matrix * vec4(in_pos, 1.0);

	v_uv = in_uv;
	v_material_id = uint(in_material_id + 0.5);
	v_ao = uint(in_ao + 0.5);
}
