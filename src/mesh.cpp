#include "mesh.h"

#include "objLoader.h"

Mesh::Mesh(const std::string &fileName)
{
	std::vector<glm::vec4> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;
	std::vector<GLushort> indices;

	loadObj(fileName, positions, normals, texCoords, indices);

	mVertices.mPositions = &positions;
	mVertices.mNormals = &normals;
	mVertices.mTextureCoordinates = &texCoords;
	mVertices.mIndices = &indices;
}

Mesh::~Mesh()
{

}

