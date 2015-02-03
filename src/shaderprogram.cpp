#include "shaderprogram.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <regex>

#include <GL/glew.h>

#include "logging.h"

using std::exit;
using std::regex;
using std::regex_match;
using std::shared_ptr;
using std::string;


ShaderProgram::ShaderProgram() :
	mId{glCreateProgram()}
{
}

ShaderProgram::~ShaderProgram()
{
	for (shared_ptr<const Shader> shader : mShaders) {
		glDetachShader(mId, shader->mId);
	}
	mShaders.clear();
	glDeleteProgram(mId);
}

void ShaderProgram::attachShader(std::shared_ptr<const Shader> shader)
{
	glAttachShader(mId, shader->mId);
	mShaders.push_back(shader);
}

void ShaderProgram::detachShader(std::shared_ptr<const Shader> shader)
{
	for (auto it = mShaders.begin(); it != mShaders.end(); ++it) {
		if (*it == shader) {
			glDetachShader(mId, shader->mId);
			mShaders.erase(it);
			return;
		}
	}
}

void ShaderProgram::vertexAttribPointer(const std::string &name, GLint size, GLenum type, GLsizei stride, GLsizei offset)
{
	GLint attrib = glGetAttribLocation(mId, name.c_str());
	glEnableVertexAttribArray(attrib);
	glVertexAttribPointer(attrib, size, type, GL_FALSE, stride, (void*) offset);
}

void ShaderProgram::bindFragDataLocation(GLuint colorNumber, const std::string &name)
{
	glBindFragDataLocation(mId, colorNumber, name.c_str());
}

void ShaderProgram::link()
{
	glLinkProgram(mId);

	GLint linkStatus;
	GLchar buffer[2048];
	static const regex regex_empty{"\\s*"};
	glGetProgramiv(mId, GL_LINK_STATUS, &linkStatus);
	glGetProgramInfoLog(mId, 2048, nullptr, buffer);
	if (!regex_match(buffer, regex_empty)) {
		WARNING("Linking shader program produced some output:\n%s", buffer);
	}
	if (linkStatus != GL_TRUE) {
		SEVERE("Could not link shader program.");
		exit(EXIT_FAILURE);
		// TODO throw exception
	}

	glValidateProgram(mId);

	GLint validateStatus;
	glGetProgramiv(mId, GL_VALIDATE_STATUS, &validateStatus);
	glGetProgramInfoLog(mId, 2048, nullptr, buffer);
	if (!regex_match(buffer, regex_empty)) {
		WARNING("Linking shader program produced some output:\n%s", buffer);
	}
	if (validateStatus != GL_TRUE) {
		SEVERE("Shader program is invalid.");
		exit(EXIT_FAILURE);
		// TODO throw exception
	}
}
