#include "objLoader.h"

#include "logging.h"

#include <regex>
#include <map>

std::regex regex("f (\\d+)//(\\d+) (\\d+)//(\\d+) (\\d+)//(\\d+)");

void loadObj(const std::string &fileName,
			 std::vector<glm::vec4> &vertices,
			 std::vector<glm::vec3> &vertexNormals,
			 std::vector<glm::vec2> &texCoords,
			 std::vector<GLushort> &vIndices)
{
	std::ifstream file;
	file.open(("./models/" + fileName + ".obj").c_str());
	if(!file.is_open())
	{
		SEVERE("unable to load ./models/%s.obg",  fileName.c_str());
		return;
	}

	std::vector<glm::vec4> vert;
	std::vector<glm::vec3> norm;

	std::map<int, GLushort> map;

	std::string line;
	int lineCount = 0;

	while(getline(file, line))
	{
		if (line.substr(0,2) == "v ") /* Vertices */
		{
		  std::istringstream s(line.substr(2));
		  glm::vec4 v; s >> v.x; s >> v.y; s >> v.z; v.w = 1.0f;
		  vert.push_back(v);
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
			norm.push_back(v);
		}  
		else if (line.substr(0,2) == "f ") /* Faces */
		{
			std::istringstream s(line.substr(2));
			GLushort a,b,c,d,e,f;

			std::cmatch m;

			std::regex_match (line.c_str(), m, regex);

			a = atoi(m[1].str().c_str())-1;
			b = atoi(m[2].str().c_str())-1;
			c = atoi(m[3].str().c_str())-1;
			d = atoi(m[4].str().c_str())-1;
			e = atoi(m[5].str().c_str())-1;
			f = atoi(m[6].str().c_str())-1;

			int x = a << 8*sizeof(GLushort) | b;
			if(map.count(x) == 0)
			{
				vIndices.push_back(vertices.size());
				map[x] = vertices.size();
				vertices.push_back(vert[a]);
				vertexNormals.push_back(norm[b]);
			}else
			{
				vIndices.push_back(map[x]);
			}
			x = c << 8*sizeof(GLushort) | d;
			if(map.count(x) == 0)
			{
				vIndices.push_back(vertices.size());
				map[x] = vertices.size();
				vertices.push_back(vert[c]);
				vertexNormals.push_back(norm[d]);
			}else
			{
				vIndices.push_back(map[x]);
			}
			x = e << 8*sizeof(GLushort) | f;
			if(map.count(x) == 0)
			{
				vIndices.push_back(vertices.size());
				map[x] = vertices.size();
				vertices.push_back(vert[e]);
				vertexNormals.push_back(norm[f]);
			}else
			{
				vIndices.push_back(map[x]);
			}
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
