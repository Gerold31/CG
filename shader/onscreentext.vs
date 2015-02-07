#version 150 core

in vec2 poscoord;
in vec2 texcoord;

out vec2 ftexcoord;

void main() {
	ftexcoord = texcoord;
	gl_Position = vec4(poscoord, 0, 1);
}
