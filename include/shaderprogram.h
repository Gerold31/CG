#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <memory>
#include <string>
#include <texture.h>
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
	void vertexAttribPointer(const std::string &name, GLint size, GLenum type, GLsizei stride, GLsizei offset);
	void bindFragDataLocation(GLuint colorNumber, const std::string &name);
	void link();

	void use() const;
	GLint getUniformLocation(const std::string &name) const;
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

inline GLint ShaderProgram::getUniformLocation(const std::string &name) const
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
