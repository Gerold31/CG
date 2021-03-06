#version 150 core

in vec3 pos;
in vec3 normal;

out vec3 fpos;
out vec3 fnormal;

uniform mat4 proj;
uniform mat4 model;

void main() {
	fnormal = normal;
	fpos = pos;
	gl_Position = proj * model * vec4(pos, 1.0);
}
