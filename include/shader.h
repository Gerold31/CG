#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

class ShaderProgram;


class Shader
{
	friend class ShaderProgram;
public:
	Shader(GLenum shaderType, const char source[] = nullptr);
    ~Shader();
	void setSource(const GLchar source[]);
	void setSource(GLsizei count, const GLchar **sources, GLint *length);
	void compile();
private:
	Shader(const Shader &) = delete;
	Shader &operator=(const Shader &) = delete;

	GLuint mId;
	GLenum mShaderType;
};

#endif // SHADER_H
