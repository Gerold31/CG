#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <istream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "mesh.h"
#include "shader.h"
#include "shaderprogram.h"
#include "texture.h"


class FileSystem
{
public:
	FileSystem(std::string root);
	virtual ~FileSystem();

	std::shared_ptr<const Mesh> getMesh(const std::string &path) const;
	std::shared_ptr<const Shader> getShader(const std::string &path) const;
	std::shared_ptr<const ShaderProgram> getShaderProgram(const std::string &path) const;
	std::shared_ptr<const Texture> getTexture(const std::string &path) const;

	std::shared_ptr<Mesh> loadMesh(const std::string &path) const;
	std::shared_ptr<Shader> loadShader(const std::string &path) const;
	std::shared_ptr<ShaderProgram> loadShaderProgram(const std::string &path) const;
	std::shared_ptr<Texture> loadTexture(const std::string &path) const;

	std::string getFullPath(const std::string &path) const;
	std::unique_ptr<std::istream> getInputStream(const std::string &path) const;
private:
	std::string mRoot;
	mutable std::unordered_map<std::string,std::shared_ptr<const Mesh>> mCacheMesh;
	mutable std::unordered_map<std::string,std::shared_ptr<const Shader>> mCacheShader;
	mutable std::unordered_map<std::string,std::shared_ptr<const ShaderProgram>> mCacheShaderProgram;
	mutable std::unordered_map<std::string,std::shared_ptr<const Texture>> mCacheTexture;
};

#endif // FILESYSTEM_H
