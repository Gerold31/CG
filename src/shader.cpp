#include "shader.h"

#include <cstdlib>
#include <iostream>
#include <regex>

#include <GL/glew.h>

using std::cerr;
using std::endl;
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
		cerr << "Compiling shader produced some output:" << endl;
		cerr << buffer << endl;
	}
	if (status != GL_TRUE) {
		// TODO throw exception
		cerr << "Could not compile shader." << endl;
		exit(EXIT_FAILURE);
	}
}
