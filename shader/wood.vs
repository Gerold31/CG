#version 150 core

in vec3 pos;
in vec3 color;
in vec3 normal;

out vec3 fpos;
out vec3 fcolor;
out vec3 fnormal;

uniform mat4 proj;

void main() {
	fcolor = color;
	fnormal = normal;
	fpos = pos;
	gl_Position = proj * vec4(pos, 1.0);
}
