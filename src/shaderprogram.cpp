#include "shaderprogram.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <regex>

#include <GL/glew.h>

using std::cerr;
using std::endl;
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

void ShaderProgram::bindFragDataLocation(GLuint colorNumber, string name)
{
	glBindFragDataLocation(mId, colorNumber, name.c_str());
}

void ShaderProgram::link()
{
	glLinkProgram(mId);

	GLint status;
	GLchar buffer[2048];
	static const regex regex_empty{"\\s*"};
	glGetProgramiv(mId, GL_LINK_STATUS, &status);
	glGetProgramInfoLog(mId, 2048, nullptr, buffer);
	if (!regex_match(buffer, regex_empty)) {
		cerr << "Linking shader program produced some output:" << endl;
		cerr << buffer << endl;
	}
	if (status != GL_TRUE) {
		// TODO throw exception
		cerr << "Could not link shader program." << endl;
		exit(EXIT_FAILURE);
	}
}
