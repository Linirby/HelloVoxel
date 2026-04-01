#version 450

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec2 in_uv;

layout(location = 0) out vec2 out_uv;

layout(set = 1, binding = 0) uniform UniformBlock { mat4 mvp; } ubo;

void main() {
	gl_Position = ubo.mvp * vec4(in_pos, 1.0);
	out_uv = in_uv;
}
