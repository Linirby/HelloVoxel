#version 450

layout(location = 0) in vec2 v_uv;

layout(set = 2, binding = 0) uniform sampler2D u_albedo_map;
layout(set = 3, binding = 0) uniform MaterialUniforms {
	vec4 color_tint;
	float roughness;
	float metallic;
	float emission;
} u_material;

layout(location = 0) out vec4 out_color;

void main() {
	vec4 tex_color = texture(u_albedo_map, v_uv);

	out_color = tex_color * u_material.color_tint;
}
