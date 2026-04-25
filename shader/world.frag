#version 450

layout(location = 0) in vec2 frag_uv;
layout(location = 1) in vec3 frag_normal;

layout(set = 2, binding = 0) uniform sampler2D texture_sampler;

layout(location = 0) out vec4 out_color;

void main() {
	vec4 tex_color = texture(texture_sampler, frag_uv);
	vec3 light_dir = normalize(vec3(0.6, 1.0, 0.4));
	float ambiant = 0.4;
	float diff = max(dot(normalize(frag_normal), light_dir), 0.0);
	float light_intensity = 0.7;
	vec3 lighting = vec3(ambiant + (diff * light_intensity));

	out_color = vec4(tex_color.rgb * lighting, tex_color.a);
}
