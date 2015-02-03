#include "shader.h"

#include <cstdlib>
#include <iostream>
#include <regex>

#include <GL/glew.h>

#include "logging.h"

using std::exit;
using std::regex;
using std::regex_match;


Shader::Shader(GLenum shaderType, const char source[]) :
	mId{glCreateShader(shaderType)},
	mShaderType{shaderType}
{
	if (source != nullptr) {
		setSource(source);
	}
}

Shader::~Shader()
{
	glDeleteShader(mId);
}

void Shader::setSource(const GLchar source[])
{
	setSource(1, &source, nullptr);
}

void Shader::setSource(GLsizei count, const GLchar **sources, GLint *length)
{
	glShaderSource(mId, count, sources, length);
}

void Shader::compile()
{
	glCompileShader(mId);

	GLint status;
	GLchar buffer[2048];
	static const regex regex_empty{"\\s*"};
	glGetShaderiv(mId, GL_COMPILE_STATUS, &status);
	glGetShaderInfoLog(mId, 2048, nullptr, buffer);
	if (!regex_match(buffer, regex_empty)) {
		WARNING("Compiling shader produced some output:\n%s", buffer);
	}
	if (status != GL_TRUE) {
		SEVERE("Could not compile shader.");
		exit(EXIT_FAILURE);
		// TODO throw exception
	}
}
