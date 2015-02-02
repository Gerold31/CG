#version 150 core

in vec3 fcolor;
in vec3 fnormal;

out vec4 pcolor;

void main() {
	pcolor = vec4(fcolor, 1.0);
}
