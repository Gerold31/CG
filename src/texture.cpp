#include "texture.h"


Texture::Texture(GLenum textureType) :
	mTextureType{textureType}
{
	glGenTextures(1, &mId);
	// TODO glCreateTexture in OpenGL 4.5+ possible
}

Texture::~Texture()
{
	glDeleteTextures(1, &mId);
}
