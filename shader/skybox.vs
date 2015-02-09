#version 150 core

in vec4 pos;

uniform vec3 cameraDir;
uniform float fov;
uniform float aspect;

out vec3 fEyeDirection;

float PI = 3.14159265358979323846264;

void main() {
	fEyeDirection.x = atan(cameraDir.z, cameraDir.x) + atan(pos.x);
	fEyeDirection.y = acos(cameraDir.y) + atan(pos.y);
	fEyeDirection.z = 0;

	gl_Position = pos;
}
