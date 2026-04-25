#version 450

layout(location = 0) in vec2 frag_uv;

layout(set = 2, binding = 0) uniform sampler2D texture_sampler;

layout(location = 0) out vec4 out_color;

void main() {
	out_color = texture(texture_sampler, frag_uv);
}
