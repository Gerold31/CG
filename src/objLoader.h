#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

#include <glm/glm.hpp>
#include <GL/glew.h>

#include "mesh.h"

void loadObj(const std::string &fileName,			// name of file	to load
			 std::vector<glm::vec4> vertices,		// output: vertex positions
			 std::vector<glm::vec3> vertexNormals,	// output: vertex normals
			 std::vector<glm::vec2> texCoords,		// output: texture coordinates for each vertex 
			 std::vector<GLushort> vIndices);		// output: indices of vertices for each face

#endif //OBJLOADER_H