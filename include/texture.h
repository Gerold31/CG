#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>


class Texture
{
public:
	Texture(GLenum textureType);
    ~Texture();
	void bind() const;
private:
	GLuint mId;
	GLenum mTextureType;
};


void Texture::bind() const
{
	glBindTexture(mTextureType, mId);
}

#endif // TEXTURE_H
