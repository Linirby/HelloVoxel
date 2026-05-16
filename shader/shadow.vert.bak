#version 450

layout(location = 0) in vec3 in_pos;

layout(set = 1, binding = 0) uniform LightMVP {
	mat4 matrix;
} u_light;

void main() {
	gl_Position = u_light.matrix * vec4(in_pos, 1.0);
}
