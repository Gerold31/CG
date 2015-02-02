#include "testbox.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "filesystem.h"


TestBox::TestBox() :
	mFS{"./"},
	mShaderProg{mFS.loadShaderProgram("shader/testbox.sp")}
{
	mShaderProg->bindFragDataLocation(0, "pcolor");
	mShaderProg->link();

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
		1.f, -1.f, -1.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f
	};
	glBufferData(GL_ARRAY_BUFFER, 12 * 9 * sizeof(GLfloat), data, GL_STATIC_DRAW);

	mShaderProg->vertexAttribPointer("pos", 3, GL_FLOAT, 9*sizeof(GLfloat), 0);
	mShaderProg->vertexAttribPointer("color", 3, GL_FLOAT, 9*sizeof(GLfloat), 3*sizeof(GLfloat));
	mShaderProg->vertexAttribPointer("normal", 3, GL_FLOAT, 9*sizeof(GLfloat), 6*sizeof(GLfloat));
}

TestBox::~TestBox()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void TestBox::update(float elapsedTime)
{
	//
}

void TestBox::draw(const Camera &camera) const
{
	mShaderProg->use();
	glBindVertexArray(vao);

	glUniformMatrix4fv(mShaderProg->getUniformLocation("proj"), 1, GL_FALSE, glm::value_ptr(camera.getProjection()));

	glDrawArrays(GL_TRIANGLES, 0, 12);
}
