#version 450

layout(location = 0) in vec3 in_pos;

layout(set = 1, binding = 0) uniform LightVP {
	mat4 view_projection;
} ubo;

void main() {
	gl_Position = ubo.view_projection * vec4(in_pos, 1.0);
}
