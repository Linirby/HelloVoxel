#version 450

layout(location = 0) in vec2 v_uv;
layout(location = 1) in vec3 v_normal;
layout(location = 2) flat in uint v_material_id;
layout(location = 3) in vec3 v_world_pos;
layout(location = 4) in vec4 v_light_pos;

layout(set = 2, binding = 0) uniform sampler2D u_albedo_map;
layout(set = 2, binding = 1) uniform sampler2DShadow u_shadow_map;

struct MaterialGPU {
	vec4 color_tint;
	float roughness;
	float metallic;
	float emission;
	float padding;
};
layout(std430, set = 2, binding = 2) readonly buffer MaterialBuffer {
	MaterialGPU materials[];
} u_materials;

layout(set = 3, binding = 0) uniform LightData {
	vec4 direction;
	vec4 color;
	vec4 ambient;
} u_light;

layout(location = 0) out vec4 out_color;

float compute_shadow(vec4 light_clip) {
	vec3 proj = light_clip.xyz / max(light_clip.w, 0.0001);
	vec2 uv = proj.xy * 0.5 + 0.5;
	uv.y = 1.0 - uv.y;
	float current_depth = proj.z;

	if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0 ||
		current_depth < 0.0 || current_depth > 1.0
	) {
		return 1.0;
	}
	
	float ndotl = max(
		dot(normalize(v_normal), -normalize(u_light.direction.xyz)), 0.0
	);
	float bias = max(0.0002 * (1.0 - ndotl), 0.00005);

	float visibility = 0.0;
	vec2 texel = 1.0 / vec2(textureSize(u_shadow_map, 0));

	for (int x = -1; x <= 1; ++x) {
		for (int y = -1; y <= 1; ++y) {
			vec2 offset = (vec2(x, y) - 0.5) * texel;
			visibility += texture(
				u_shadow_map, vec3(uv + offset, current_depth - bias)
			);
		}
	}
	return visibility / 9.0;
}

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

	float shadow = 1.0f;
	if (diff > 0.0001)
		shadow = compute_shadow(v_light_pos);

	out_color = vec4(
		ambient + shadow * diffuse + emissive,
		tex_color.a * material.color_tint.a
	);
}
