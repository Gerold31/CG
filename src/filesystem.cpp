#include "filesystem.h"

#include <cstdlib>
#include <cerrno>
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
#include "types.h"

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
	mRoot(root[root.length()-1] == '/' ? root : root.substr(0, root.length()-1))
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

	static const std::regex regex_comment{"#.*|"};
	static const std::regex regex_option{"\\s*(\\S*)\\s*=\\s*(\\S*)\\s*"};
	static const std::regex regex_section{"\\s*\\[\\s*(\\S*)\\s*\\]\\s*"};

	auto stream = getInputStream(path);
	std::shared_ptr<ShaderProgram> prog = std::make_shared<ShaderProgram>();

	struct fb_tex_entry {
		std::string name;
		GLint location;
	};
	std::vector<fb_tex_entry> framebufferBindings;
	std::vector<fb_tex_entry> textureBindings;

	unsigned int lineNr = 0;
	std::string line;
	std::string section{""};
	int sectionId = 0;
	while (stream->good()) {
		++lineNr;
		std::getline(*stream, line);
		std::smatch match;
		if (std::regex_match(line, match, regex_section)) {
			section = match[1];
			if(section == "shader") {
				sectionId = 1;
			} else if (section == "texture") {
				sectionId = 2;
			} else if (section == "framebuffer") {
				sectionId = 3;
			} else {
				WARNING("Unknown section in shader program: %s ('%s')", path.c_str(), section.c_str());
				sectionId = 0;
			}
		} else if (std::regex_match(line, match, regex_option)) {
			std::string key = match[1];
			std::string value = match[2];
			if (sectionId == 1) {
				// [ shader ]
				auto shader = getShader(value);
				prog->attachShader(shader);
			} else if (sectionId == 2) {
				// [ texture ]
				errno = 0;
				fb_tex_entry entry;
				entry.location = (GLint) strtol(value.c_str(), nullptr, 0);
				if (errno != 0) {
					WARNING("Inavlid value on line %d in '%s': %s", lineNr, path.c_str(), strerror(errno));
					continue;
				} else if (entry.location < 1) {
					WARNING("Invalid texture slot on line %d in %s", lineNr, path.c_str());
					continue;
				}
				entry.name = key;
				textureBindings.push_back(entry);
			} else if (sectionId == 3) {
				// [ framebuffer ]
				errno = 0;
				fb_tex_entry entry;
				entry.location = (GLint) strtol(value.c_str(), nullptr, 0);
				if (errno != 0) {
					WARNING("Inavlid value on line %d in '%s': %s", lineNr, path.c_str(), strerror(errno));
					continue;
				} else if (entry.location < 0) {
					WARNING("Invalid fragment data location on line %d in '%s'", lineNr, path.c_str());
					continue;
				}
				entry.name = key;
				framebufferBindings.push_back(entry);
			}
		} else if (!std::regex_match(line, regex_comment)) {
			WARNING("Invalid line in shader program: %s ('%s')", path.c_str(), line.c_str());
		}
	}

	// TODO check if stream has finished or error occurred.

	for (const fb_tex_entry &entry : framebufferBindings) {
		FINE("Set frag data location: %s to %d", entry.name.c_str(), entry.location);
		prog->bindFragDataLocation(entry.location, entry.name);
	}
	prog->link();
	for (const fb_tex_entry &entry : textureBindings) {
		FINE("Set uniform '%s' to %d.", entry.name.c_str(), entry.location);
		prog->setUniform(entry.name, entry.location);
	}

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
			WARNING("Image's' width is not a power of 2: %s", path.c_str());
		}
		// Also check if the height is a power of 2
		if ( (surface->h & (surface->h - 1)) != 0 ) {
			WARNING("Image's height is not a power of 2: %s", path.c_str());
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
			WARNING("Image is not truecolor: %s", path.c_str());
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
	std::unique_ptr<std::ifstream> stream(new std::ifstream(getFullPath(path)));
	// TODO throw on error
	return std::move(stream);
}
