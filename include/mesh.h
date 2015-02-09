#ifndef MESH_H
#define MESH_H

#include <vector>

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>

#include "types.h"

struct Vertices
{
	std::vector<Vec4> mPositions;
	std::vector<Vec3> mNormals;
	std::vector<Vec2> mTextureCoordinates;
	std::vector<GLushort> mIndices;
};

class Mesh
{
public:
    Mesh(const std::string &fileName);
    ~Mesh();

	Vertices &getVertices() { return mVertices; }
private:
	Vertices mVertices;
};

#endif // MESH_H
