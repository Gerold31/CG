#version 150 core

in vec3 pos;
in vec3 color;
in vec3 normal;
in float seed;

out vec3 fpos;
out vec3 fcolor;
out vec3 fnormal;
out float fseed;

uniform mat4 proj;
uniform mat4 model;

void main() {
	fcolor = color;
	fnormal = normal;
	fpos = pos;
	fseed = seed;
	gl_Position = proj * model * vec4(pos, 1.0);
}
