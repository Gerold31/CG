#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "types.h"


class ShaderProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();
	void attachShader(std::shared_ptr<const Shader> shader);
	void detachShader(std::shared_ptr<const Shader> shader);
	void bindFragDataLocation(GLuint colorNumber, const std::string &name);
	void link();

	void use() const;
	GLint getUniformLocation(const std::string &name) const;
	void setVertexAttribPointer(const std::string &name, GLint size, GLenum type, GLsizei stride, GLsizei offset) const;

	void setUniform(const std::string &name, GLint value) const;
	void setUniform(const std::string &name, GLfloat value) const;
	void setUniform(const std::string &name, const Vec3 &value) const;
	void setUniform(const std::string &name, const Mat3 &value) const;
	void setUniform(const std::string &name, const Mat4 &value) const;

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

inline void ShaderProgram::setVertexAttribPointer(const std::string &name, GLint size, GLenum type, GLsizei stride, GLsizei offset) const
{
	GLint attrib = glGetAttribLocation(mId, name.c_str());
	glEnableVertexAttribArray(attrib);
	glVertexAttribPointer(attrib, size, type, GL_FALSE, stride, (GLvoid*) offset);
}

inline void ShaderProgram::setUniform(const std::string &name, GLint value) const
{
	glUniform1i(getUniformLocation(name), value);
}

inline void ShaderProgram::setUniform(const std::string &name, GLfloat value) const
{
	glUniform1f(getUniformLocation(name), value);
}

inline void ShaderProgram::setUniform(const std::string &name, const Vec3 &value) const
{
	glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

inline void ShaderProgram::setUniform(const std::string &name, const Mat3 &value) const
{
	glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

inline void ShaderProgram::setUniform(const std::string &name, const Mat4 &value) const
{
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

#endif // SHADERPROGRAM_H
