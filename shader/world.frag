#version 450

layout(location = 0) in vec3 v_normal;
layout(location = 1) in vec2 v_uv;
layout(location = 2) flat in uint v_material_id;
layout(location = 3) in float v_ao;

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

layout(set = 3, binding = 0) uniform LightData {
	vec4 direction;
	vec4 color;
	vec4 ambient;
} u_light;

layout(location = 0) out vec4 out_color;

void main() {
	vec4 tex_color = texture(u_albedo_map, v_uv);
	MaterialGPU material = u_materials.materials[v_material_id];

	vec3 albedo = tex_color.rgb * material.color_tint.rgb;
	vec3 emissive = albedo * material.emission;

	vec3 normal = normalize(v_normal);
	vec3 light_dir = normalize(-u_light.direction.xyz);
	float diff_factor = max(dot(normal, light_dir), 0.0);
	vec3 diffuse_light = diff_factor * u_light.color.rgb * u_light.color.a;

	float ao_multiplier = 1.0 - (v_ao / 3.0) * 0.8;

	vec3 final_lighting = (u_light.ambient.rgb + diffuse_light) * ao_multiplier;
	vec3 final_color = tex_color.rgb * final_lighting;

	out_color = vec4(final_color, tex_color.a);
}
