#version 450

layout(location = 0) in vec3 in_pos;
layout(location = 2) in vec2 in_uv;
layout(location = 3) in float in_material_id;

layout(location = 0) out vec2 v_uv;
layout(location = 2) flat out uint v_material_id;

layout(set = 1, binding = 0) uniform UniformBlock {
	mat4 matrix;
} ubo;

void main() {
	gl_Position = ubo.matrix * vec4(in_pos, 1.0);
	v_uv = in_uv;
	v_material_id = uint(in_material_id + 0.5);
}
