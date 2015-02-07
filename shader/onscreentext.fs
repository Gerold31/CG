#version 150 core
#extension GL_ARB_shading_language_420pack : enable

in vec2 ftexcoord;
out vec4 pcolor;

layout(binding=1) uniform sampler2D glyph;

void main() {
	pcolor = vec4(1, 1, 1, texture(glyph, ftexcoord).r);
}
