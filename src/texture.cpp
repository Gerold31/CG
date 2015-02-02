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

void Texture::setTexImage2D(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *data)
{
	bind();
	glTexImage2D(mTextureType, 0, internalFormat, width, height, 0, format, type, data);
}
