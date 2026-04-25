#version 450

layout(location = 0) in vec3 in_pos;
layout(location = 2) in vec2 in_uv;

layout(location = 0) out vec2 frag_uv;

layout(set = 1, binding = 0) uniform UniformBlock {
	mat4 matrix;
} ubo;

void main() {
	gl_Position = ubo.matrix * vec4(in_pos, 1.0);
	frag_uv = in_uv;
}
