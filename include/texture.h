#ifndef TEXTURE_H
#define TEXTURE_H

#include <cassert>

#include <GL/glew.h>


class Texture
{
public:
	Texture(GLenum textureType);
    ~Texture();

	void bind();
	void setTexParameter(GLenum parameter, GLfloat value);
	void setTexParameter(GLenum parameter, GLint value);
	void setTexParameter(GLenum parameter, const GLfloat *value);
	void setTexParameter(GLenum parameter, const GLint *value);
	void setTexImage2D(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *data);

	void bindToSlot(int index) const;
private:
	Texture(const Texture &); // = delete;
	Texture &operator=(const Texture &); // = delete;

	GLuint mId;
	GLenum mTextureType;
};


inline void Texture::bind()
{
	glBindTexture(mTextureType, mId);
}

inline void Texture::setTexParameter(GLenum parameter, GLfloat value)
{
	bind();
	glTexParameterf(mTextureType, parameter, value);
}

inline void Texture::setTexParameter(GLenum parameter, GLint value)
{
	bind();
	glTexParameteri(mTextureType, parameter, value);
}

inline void Texture::setTexParameter(GLenum parameter, const GLfloat *value)
{
	bind();
	glTexParameterfv(mTextureType, parameter, value);
}

inline void Texture::setTexParameter(GLenum parameter, const GLint *value)
{
	bind();
	glTexParameteriv(mTextureType, parameter, value);
}

inline void Texture::bindToSlot(int index) const
{
	assert(index > 0);
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(mTextureType, mId);
	glActiveTexture(GL_TEXTURE0);
}

#endif // TEXTURE_H
