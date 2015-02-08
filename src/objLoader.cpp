#include "objLoader.h"

void loadObj(const std::string &fileName, 
			 std::vector<glm::vec4> vertices, 
			 std::vector<glm::vec3> vertexNormals, 
			 std::vector<glm::vec2> texCoords, 
			 std::vector<GLushort> vIndices)
{
	std::ifstream file;
	file.open(("./models/" + fileName + ".obj").c_str());
	if(!file.is_open())
		std::cerr << "ERROR: unable to load ./res/" << fileName << ".obj" << std::endl;

	std::string line;
	int lineCount = 0;

	while(getline(file, line))
	{
		if (line.substr(0,2) == "v ") /* Vertices */
		{
		  std::istringstream s(line.substr(2));
		  glm::vec4 v; s >> v.x; s >> v.y; s >> v.z; v.w = 1.0f;
		  vertices.push_back(v);
		}
		else if (line.substr(0,3) == "vt ") /* Texture Coordinates */
		{
			std::istringstream s(line.substr(3));
			glm::vec2 v; s >> v.x; s >> v.y;
			texCoords.push_back(v);
		}  
		else if (line.substr(0,3) == "vn ") /* Vertex Normals */
		{
			std::istringstream s(line.substr(3));
			glm::vec3 v; s >> v.x; s >> v.y; s >> v.z;
			vertexNormals.push_back(v);
		}  
		else if (line.substr(0,2) == "f ") /* Faces */
		{
		  std::istringstream s(line.substr(2));
		  GLushort a,b,c;
		  s >> a; s >> b; s >> c;
		  a--; b--; c--;
		  vIndices.push_back(a); vIndices.push_back(b); vIndices.push_back(c);
		}
		else if (line[0] == '#') { /* Comment */ }
		else { /* anything else */ }
	}

// this is for surface normals, if needed
//	normals.resize(vertices.size(), glm::vec3(0.0, 0.0, 0.0));
//	for (int i = 0; i < vIndices.size(); i+=3) 
//	{
//		GLushort ia = vIndices[i];
//		GLushort ib = vIndices[i+1];
//		GLushort ic = vIndices[i+2];
//		glm::vec3 normal = glm::normalize(glm::cross(glm::vec3(vertices[ib]) - glm::vec3(vertices[ia]), glm::vec3(vertices[ic]) - glm::vec3(vertices[ia])));
//		normals[ia] = normals[ib] = normals[ic] = normal;
//	}
}