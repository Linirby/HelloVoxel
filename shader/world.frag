#version 450

layout(location = 0) in vec2 v_uv;
layout(location = 1) in vec3 v_normal;
layout(location = 2) flat in uint v_material_id;
layout(location = 3) in vec3 v_world_pos;

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
	vec3 normal = normalize(v_normal);
	vec3 ambient = u_light.ambient.xyz * albedo;
	float diff = max(dot(normal, -u_light.direction.xyz), 0.0);
	vec3 diffuse = diff * u_light.color.rgb * u_light.color.a * albedo;
	float smoothness = 1.0 - material.roughness;
	vec3 specular_color = mix(vec3(1.0), albedo, material.metallic);
	vec3 emissive = albedo * material.emission;

	out_color = vec4(
		ambient + diffuse + emissive, tex_color.a * material.color_tint.a
	);
}
