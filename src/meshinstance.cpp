#include "meshinstance.h"

MeshInstance::MeshInstance(Mesh &mesh)
{
	glGenBuffers(NUMBUFFERS, mVertexArrayBuffers);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexArrayBuffers[POSITIONVB]);
	glVertexAttribPointer(
		0,					// attribute: vertex position
		4,                  // (x,y,z,w)
		GL_FLOAT,           // type of elements
		GL_FALSE,           // take values as-is
		0,                  // no extra data between each position
		0                   // offset of first element
	);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexArrayBuffers[TEXCOORDVB]);
	glVertexAttribPointer(
		1,					// attribute: texture coordinate
		2,					// (x,y)
		GL_FLOAT,			// type of elements
		GL_FALSE,			// take value as-is
		0,					// no extra data between each position
		0					// offset of first element
		);
 
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexArrayBuffers[NORMALVB]);
	glVertexAttribPointer(
		2,					// attribute: vertex normal
		3,                  // (x,y,z)
		GL_FLOAT,           // type of elements
		GL_FALSE,           // take values as-is
		0,                  // no extra data between each position
		0                   // offset of first element
	);
 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVertexArrayBuffers[INDEXVB]);	
}

MeshInstance::~MeshInstance()
{
	glDeleteBuffers(NUMBUFFERS, mVertexArrayBuffers);
}

void MeshInstance::draw(const Camera &camera) const
{
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);  
	glDrawElements(
		GL_TRIANGLES,					// mode
		size/sizeof(GLushort),			// draw count
		GL_UNSIGNED_SHORT,				// taype of indices
		&mVertexArrayBuffers[INDEXVB]);	// pointer to location where Indices are stored
}

void MeshInstance::update(float elapsedTime)
{
}