#include "meshinstance.h"

#include "logging.h"
#include "camera.h"
#include "scene.h"
#include "light.h"

MeshInstance::MeshInstance(std::shared_ptr<Mesh> mesh, bool black) :
	mFS("./"),
	mShaderProg(mFS.getShaderProgram("shader/wood.sp"))
{
	/*
	mSeed = rand()%(1<<16);
	if(black)
		mColor = Color(150/256., 100/256., 50/256.);
	else
		mColor = Color(238/256., 197/256., 145/256.);
	*/
	mMesh = mesh;
	int numVertices = mMesh->getVertices().mPositions.size();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(NUMBUFFERS, mVertexArrayBuffers);

	//glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexArrayBuffers[POSITIONVB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec4), mMesh->getVertices().mPositions.data(), GL_STATIC_DRAW);
	/*
	glVertexAttribPointer(
		0,					// attribute: vertex position
		4,                  // (x,y,z,w)
		GL_FLOAT,           // type of elements
		GL_FALSE,           // take values as-is
		0,                  // no extra data between each position
		0                   // offset of first element
	);*/

	mShaderProg->setVertexAttribPointer("pos", 4, GL_FLOAT, 4*sizeof(GLfloat), 0);


	/*
	//glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexArrayBuffers[TEXCOORDVB]);
	glBufferData(GL_ARRAY_BUFFER, mMesh->getVertices().mTextureCoordinates.size() * sizeof(glm::vec2), mMesh->getVertices().mTextureCoordinates.data(), GL_STATIC_DRAW);
	/*
	glVertexAttribPointer(
		1,					// attribute: texture coordinate
		2,					// (x,y)
		GL_FLOAT,			// type of elements
		GL_FALSE,			// take value as-is
		0,					// no extra data between each position
		0					// offset of first element
		);
	*

	mShaderProg->setVertexAttribPointer("texcoord", 2, GL_FLOAT, 2*sizeof(GLfloat), 0);
	*/

	//glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexArrayBuffers[NORMALVB]);
	glBufferData(GL_ARRAY_BUFFER, mMesh->getVertices().mNormals.size() * sizeof(glm::vec3), mMesh->getVertices().mNormals.data(), GL_STATIC_DRAW);
	/*
	glVertexAttribPointer(
		2,					// attribute: vertex normal
		3,                  // (x,y,z)
		GL_FLOAT,           // type of elements
		GL_FALSE,           // take values as-is
		0,                  // no extra data between each position
		0                   // offset of first element
	);
	*/

	mShaderProg->setVertexAttribPointer("normal", 3, GL_FLOAT, 3*sizeof(GLfloat), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVertexArrayBuffers[INDEXVB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mMesh->getVertices().mIndices.size() * sizeof(GLushort), mMesh->getVertices().mIndices.data(), GL_STATIC_DRAW);

}

MeshInstance::~MeshInstance()
{
	glDeleteBuffers(NUMBUFFERS, mVertexArrayBuffers);
}

void MeshInstance::draw(const Camera &camera) const
{
	mShaderProg->use();
	glBindVertexArray(vao);

	mShaderProg->setUniform("proj", camera.getProjection());
	mShaderProg->setUniform("model", getTransfToGlobale());

	/*
	mShaderProg->setUniform("seed", mSeed);
	mShaderProg->setUniform("color", mColor);
	*/

	//lighting
	mShaderProg->setUniform("camPos", (Vec3)camera.getPosition());
	mShaderProg->setUniform("ambientLight", getScene()->getAmbientLight());

	size_t i = getScene()->getNumLights();
	mShaderProg->setUniform("numLights", (int)i);
	for(size_t i=0; i<getScene()->getNumLights(); i++)
	{
		shared_ptr<Light> l = getScene()->getLight(i);

		mShaderProg->setUniform("lights", i, "position", l->getPosition());
		mShaderProg->setUniform("lights", i, "color", l->getColor());
		mShaderProg->setUniform("lights", i, "attenuation", l->getAttenuation());
	}

	int size = mMesh->getVertices().mIndices.size();
	//glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVertexArrayBuffers[INDEXVB]);
	glDrawElements(
		GL_TRIANGLES,					// mode
		size,			// draw count
		GL_UNSIGNED_SHORT,				// taype of indices
		0);//mMesh->getVertices().mIndices.data());	// offset //pointer to location where Indices are stored
}

void MeshInstance::update(float elapsedTime)
{
}
