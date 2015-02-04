#include "testbox.h"

#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "filesystem.h"


TestBox::TestBox() :
	mFS("./"),
	mShaderProg(mFS.getShaderProgram("shader/testbox.sp"))
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	GLfloat data[] = {
		-1.f, -1.f, -1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		1.f, -1.f, -1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		1.f, 1.f, -1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		1.f, 1.f, -1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		-1.f, 1.f, -1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		-1.f, -1.f, -1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f,

		1.f, -1.f, -1.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f,
		1.f, -1.f, 1.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f,
		1.f, 1.f, 1.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f,
		1.f, 1.f, 1.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f,
		1.f, 1.f, -1.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f,
		1.f, -1.f, -1.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f,

		1.f, -1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f,
		-1.f, -1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f,
		-1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f,
		-1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f,
		1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f,
		1.f, -1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f,

		-1.f, -1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f,
		-1.f, -1.f, -1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f,
		-1.f, 1.f, -1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f,
		-1.f, 1.f, -1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f,
		-1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f,
		-1.f, -1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f,

		-1.f, -1.f, -1.f, 0.5f, 0.5f, 0.5f, 0.f, 0.f, 0.f,
		-1.f, -1.f, 1.f, 0.5f, 0.5f, 0.5f, 0.f, 0.f, 0.f,
		1.f, -1.f, 1.f, 0.5f, 0.5f, 0.5f, 0.f, 0.f, 0.f,
		1.f, -1.f, 1.f, 0.5f, 0.5f, 0.5f, 0.f, 0.f, 0.f,
		1.f, -1.f, -1.f, 0.5f, 0.5f, 0.5f, 0.f, 0.f, 0.f,
		-1.f, -1.f, -1.f, 0.5f, 0.5f, 0.5f, 0.f, 0.f, 0.f,

		-1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f,
		-1.f, 1.f, -1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f,
		1.f, 1.f, -1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f,
		1.f, 1.f, -1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f,
		-1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f,
		-1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f
	};
	glBufferData(GL_ARRAY_BUFFER, 36 * 9 * sizeof(GLfloat), data, GL_STATIC_DRAW);

	mShaderProg->setVertexAttribPointer("pos", 3, GL_FLOAT, 9*sizeof(GLfloat), 0);
	mShaderProg->setVertexAttribPointer("color", 3, GL_FLOAT, 9*sizeof(GLfloat), 3*sizeof(GLfloat));
	mShaderProg->setVertexAttribPointer("normal", 3, GL_FLOAT, 9*sizeof(GLfloat), 6*sizeof(GLfloat));
}

TestBox::~TestBox()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void TestBox::update(float elapsedTime)
{
	rotate(M_PI_4*elapsedTime, Vec3(0.f, 1.f, 0.f));
}

void TestBox::draw(const Camera &camera) const
{
	mShaderProg->use();
	glBindVertexArray(vao);

	mShaderProg->setUniform("proj", camera.getProjection());
	mShaderProg->setUniform("model", getTransfToGlobale());

	glDrawArrays(GL_TRIANGLES, 0, 36);
}
