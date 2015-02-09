#ifndef MESHINSTANCE_H
#define MESHINSTANCE_H

#include <glm/glm.hpp>
#include <GL/glew.h>

#include "drawable.h"
#include "mesh.h"
#include "shaderprogram.h"
#include "filesystem.h"

class Mesh;
class Material;

class MeshInstance : public Drawable
{
public:
	MeshInstance(std::shared_ptr<Mesh> mesh);
    ~MeshInstance();

    virtual void draw(const Camera &camera) const;
	virtual void update(float elapsedTime);

private:
	std::shared_ptr<Mesh> mMesh;
	FileSystem mFS;
	shared_ptr<const ShaderProgram> mShaderProg;
	GLuint vao;

	enum
	{
		POSITIONVB,			// Vertex Array Buffer for vertex positions
		TEXCOORDVB,			// Vertex Array Buffer for texture coordinates
		NORMALVB,			// Vertex Array Buffer for vertex normals
		INDEXVB,			// Vertex Array Buffer for vertex indices
		NUMBUFFERS			// number of Buffers
	};

	GLuint mVertexArrayBuffers[NUMBUFFERS];		// Buffer Objects
};

#endif // MESHINSTANCE_H
