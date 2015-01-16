#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <GL/glew.h>

#include "shader.h"


class ShaderProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();
	void attachShader(std::shared_ptr<const Shader> shader);
	void detachShader(std::shared_ptr<const Shader> shader);
	void bindFragDataLocation(GLuint colorNumber, std::string name);
	void link();
	void use() const;
	GLint getUniformLocation(const std::string name) const;
private:
	ShaderProgram(const ShaderProgram &) = delete;
	ShaderProgram &operator=(const ShaderProgram &) = delete;

	GLuint mId;
	std::vector<std::shared_ptr<const Shader>> mShaders;
	mutable std::unordered_map<std::string,GLint> mUniforms;
};


inline void ShaderProgram::use() const
{
	glUseProgram(mId);
}

GLint ShaderProgram::getUniformLocation(const std::string name) const
{
	auto it = mUniforms.find(name);
	if (it == mUniforms.end()) {
		GLint l = glGetUniformLocation(mId, name.c_str());
		mUniforms[name] = l;
		return l;
	} else {
		return it->second;
	}
}

#endif // SHADERPROGRAM_H
