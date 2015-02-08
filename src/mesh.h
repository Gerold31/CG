#ifndef MESH_H
#define MESH_H

#include <vector>

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>

struct Vertices
{
	std::vector<glm::vec4> *mPositions;
	std::vector<glm::vec3> *mNormals;
	std::vector<glm::vec2> *mTextureCoordinates;
	std::vector<GLushort> *mIndices;
};

class Mesh
{
public:
    Mesh(const std::string &fileName);
    ~Mesh();

	Vertices getVertices() { return mVertices; }
private:
	Vertices mVertices;
};

#endif // MESH_H
