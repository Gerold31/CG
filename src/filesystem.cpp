#include "filesystem.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>

#include <GL/glew.h>

#include <SDL2/SDL.h>

#include "logging.h"
#include "mesh.h"
#include "shader.h"
#include "shaderprogram.h"
#include "texture.h"

#define GET(type, var, path, cache, load) std::shared_ptr<const type> var; \
{ \
	auto it = cache.find(path); \
	if (it != cache.end()) { \
		var = it->second; \
	} else { \
		var = load(path); \
		cache[path] = var; \
	} \
}


FileSystem::FileSystem(std::string root) :
	mRoot{root[root.length()-1] == '/' ? root : root.substr(0, root.length()-1)}
{

}

FileSystem::~FileSystem()
{

}

std::shared_ptr<const Mesh> FileSystem::getMesh(const std::string &path) const
{
	GET(Mesh, var, path, mCacheMesh, loadMesh);
	return var;
}

std::shared_ptr<const Shader> FileSystem::getShader(const std::string &path) const
{
	GET(Shader, var, path, mCacheShader, loadShader);
	return var;
}

std::shared_ptr<const ShaderProgram> FileSystem::getShaderProgram(const std::string &path) const
{
	GET(ShaderProgram, var, path, mCacheShaderProgram, loadShaderProgram);
	return var;
}

std::shared_ptr<const Texture> FileSystem::getTexture(const std::string &path) const
{
	GET(Texture, var, path, mCacheTexture, loadTexture);
	return var;
}

std::shared_ptr<Mesh> FileSystem::loadMesh(const std::string &path) const
{
	// TODO
}

std::shared_ptr<Shader> FileSystem::loadShader(const std::string &path) const
{
	INFO("Load shader: %s", path.c_str());
	auto stream = getInputStream(path);
	std::stringstream buffer;
	buffer << stream->rdbuf();
	std::string source{buffer.str()};

	GLenum type;
	if (path.substr(path.length() - 3) == ".vs") {
		type = GL_VERTEX_SHADER;
	} else if (path.substr(path.length() - 3) == ".fs") {
		type = GL_FRAGMENT_SHADER;
	} else {
		WARNING("Unknown type of shader: %s", path.c_str());
		// TODO add other types and throw when nothing metches...
	}

	std::shared_ptr<Shader> shader = std::make_shared<Shader>(type, source.c_str());
	shader->compile();
	return std::move(shader);
}

std::shared_ptr<ShaderProgram> FileSystem::loadShaderProgram(const std::string &path) const
{
	INFO("Load shader program: %s", path.c_str());
	static const std::regex r{"\\s*(\\S*)\\s*=\\s*(\\S*)\\s*"};
	auto stream = getInputStream(path);

	std::shared_ptr<ShaderProgram> prog = std::make_shared<ShaderProgram>();

	std::string line;
	while (stream->good()) {
		std::getline(*stream, line);
		std::smatch match;
		if (std::regex_match(line, match, r)) {
			auto shader = getShader(match[2]);
			prog->attachShader(shader);
		} else {
			WARNING("Invalid line in shader program: %s ('%s')", path.c_str(), line.c_str());
			// TODO thow exception?
		}
	}

	// TODO bind fragment data location

	prog->link();
	return std::move(prog);
}

std::shared_ptr<Texture> FileSystem::loadTexture(const std::string &path) const
{
	INFO("Load texture: %s", path.c_str());
	SDL_Surface *surface;
	std::shared_ptr<Texture> texture = nullptr;
	if (surface = SDL_LoadBMP(getFullPath(path).c_str()))
	{
		// Check that the image's width is a power of 2
		if ( (surface->w & (surface->w - 1)) != 0 ) {
			std::cerr << "warning: image's' width is not a power of 2: " << path << std::endl;
		}
		// Also check if the height is a power of 2
		if ( (surface->h & (surface->h - 1)) != 0 ) {
			std::cerr << "warning: image's height is not a power of 2: " << path << std::endl;
		}
		// get the number of channels in the SDL surface
		int channels = surface->format->BytesPerPixel;
		// get texture format
		GLenum type;
		if (channels == 4) {
			if (surface->format->Rmask == 0x000000ff)
				type = GL_RGBA;
			else
				type = GL_BGRA;
		} else if (channels == 3) {
			if (surface->format->Rmask == 0x000000ff)
				type = GL_RGB;
			else
				type = GL_BGR;
		} else {
			WARNING("the image is not truecolor: %s", path.c_str());
			// TODO error handling
		}
		// create texture
		texture = std::make_shared<Texture>(GL_TEXTURE_2D);
		texture->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		texture->setTexParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		texture->setTexImage2D(channels, surface->w, surface->h, type, GL_UNSIGNED_BYTE, surface->pixels);

		// Free SDL surface
		SDL_FreeSurface(surface);
	}
	else
	{
		WARNING("Could not load texture: %s", path.c_str());
	}

	return std::move(texture);
}

std::string FileSystem::getFullPath(const std::string &path) const
{
	if (path[0] == '/' || path[0] == '\\') {
		return mRoot + path.substr(1);
	} else {
		return mRoot + path;
	}
}

std::unique_ptr<std::istream> FileSystem::getInputStream(const std::string &path) const
{
	std::unique_ptr<std::ifstream> stream{new std::ifstream{getFullPath(path)}};
	// TODO throw on error
	return std::move(stream);
}
