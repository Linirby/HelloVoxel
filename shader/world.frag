#version 450

layout(location = 0) in vec2 v_uv;
layout(location = 1) flat in uint v_material_id;
layout(location = 2) in float v_ao;

layout(set = 2, binding = 0) uniform sampler2D u_albedo_map;

struct MaterialGPU {
	vec4 color_tint;
	float roughness;
	float metallic;
	float emission;
	float padding;
};
layout(std430, set = 2, binding = 1) readonly buffer MaterialBuffer {
	MaterialGPU materials[];
} u_materials;

layout(location = 0) out vec4 out_color;

void main() {
	vec4 tex_color = texture(u_albedo_map, v_uv);
	MaterialGPU material = u_materials.materials[v_material_id];

	vec3 albedo = tex_color.rgb * material.color_tint.rgb;
	vec3 emissive = albedo * material.emission;

	float ao = 1.0 - (v_ao / 3.0) * 0.75;
	vec3 ambient = albedo * ao;

	out_color = vec4(
		ambient + emissive,
		tex_color.a * material.color_tint.a
	);
}
