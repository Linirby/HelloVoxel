#version 450

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec4 in_color;

layout(set = 1, binding = 0) uniform UniformBlock {
	mat4 mvp;
} ubo;

layout(location = 0) out vec4 frag_color;

void main() {
	gl_Position = ubo.mvp * vec4(in_pos, 1.0);
	frag_color = in_color;
}
