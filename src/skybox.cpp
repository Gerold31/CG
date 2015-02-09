#include "skybox.h"

#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "filesystem.h"
#include "scene.h"
#include "logging.h"

#define MIN_X (-1.f)
#define MAX_X ( 1.f)
#define MIN_Y (-1.f)
#define MAX_Y ( 1.f)
#define NUM_VERTICES_X (1)
#define NUM_VERTICES_Y (1)
#define NUM_VERTICES (NUM_VERTICES_X*NUM_VERTICES_Y*6)

SkyBox::SkyBox() :
	mFS("./"),
	mShaderProg(mFS.getShaderProgram("shader/skybox.sp"))
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	GLfloat *data = new GLfloat [NUM_VERTICES * 4];

	for(int x=0; x<NUM_VERTICES_X; x++)
	{
		for(int y=0; y<NUM_VERTICES_Y; y++)
		{
			float lowerX = (MAX_X-MIN_X)*(float)(x)/NUM_VERTICES_X+MIN_X;
			float upperX = (MAX_X-MIN_X)*(float)(x+1)/NUM_VERTICES_X+MIN_X;
			float lowerY = (MAX_Y-MIN_Y)*(float)(y)/NUM_VERTICES_Y+MIN_Y;
			float upperY = (MAX_Y-MIN_Y)*(float)(y+1)/NUM_VERTICES_Y+MIN_Y;
			data[(x*NUM_VERTICES_Y + y) * 6*4 + 0*4 + 0] = lowerX;
			data[(x*NUM_VERTICES_Y + y) * 6*4 + 0*4 + 1] = lowerY;
			data[(x*NUM_VERTICES_Y + y) * 6*4 + 0*4 + 2] = 0.f;
			data[(x*NUM_VERTICES_Y + y) * 6*4 + 0*4 + 3] = 1.f;
			data[(x*NUM_VERTICES_Y + y) * 6*4 + 1*4 + 0] = upperX;
			data[(x*NUM_VERTICES_Y + y) * 6*4 + 1*4 + 1] = lowerY;
			data[(x*NUM_VERTICES_Y + y) * 6*4 + 1*4 + 2] = 0.f;
			data[(x*NUM_VERTICES_Y + y) * 6*4 + 1*4 + 3] = 1.f;
			data[(x*NUM_VERTICES_Y + y) * 6*4 + 2*4 + 0] = upperX;
			data[(x*NUM_VERTICES_Y + y) * 6*4 + 2*4 + 1] = upperY;
			data[(x*NUM_VERTICES_Y + y) * 6*4 + 2*4 + 2] = 0.f;
			data[(x*NUM_VERTICES_Y + y) * 6*4 + 2*4 + 3] = 1.f;
			data[(x*NUM_VERTICES_Y + y) * 6*4 + 3*4 + 0] = upperX;
			data[(x*NUM_VERTICES_Y + y) * 6*4 + 3*4 + 1] = upperY;
			data[(x*NUM_VERTICES_Y + y) * 6*4 + 3*4 + 2] = 0.f;
			data[(x*NUM_VERTICES_Y + y) * 6*4 + 3*4 + 3] = 1.f;
			data[(x*NUM_VERTICES_Y + y) * 6*4 + 4*4 + 0] = lowerX;
			data[(x*NUM_VERTICES_Y + y) * 6*4 + 4*4 + 1] = upperY;
			data[(x*NUM_VERTICES_Y + y) * 6*4 + 4*4 + 2] = 0.f;
			data[(x*NUM_VERTICES_Y + y) * 6*4 + 4*4 + 3] = 1.f;
			data[(x*NUM_VERTICES_Y + y) * 6*4 + 5*4 + 0] = lowerX;
			data[(x*NUM_VERTICES_Y + y) * 6*4 + 5*4 + 1] = lowerY;
			data[(x*NUM_VERTICES_Y + y) * 6*4 + 5*4 + 2] = 0.f;
			data[(x*NUM_VERTICES_Y + y) * 6*4 + 5*4 + 3] = 1.f;
		}
	}

	glBufferData(GL_ARRAY_BUFFER, NUM_VERTICES * 4 * sizeof(GLfloat), data, GL_STATIC_DRAW);

	mShaderProg->setVertexAttribPointer("pos", 4, GL_FLOAT, 4*sizeof(GLfloat), 0);

	delete[] data;
}

SkyBox::~SkyBox()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void SkyBox::update(float elapsedTime)
{
	mTime += elapsedTime;
}

void SkyBox::draw(const Camera &camera) const
{
	//glDepthRange(1.f, 1.f);
	glDisable(GL_DEPTH_TEST);

	mShaderProg->use();
	glBindVertexArray(vao);

	mShaderProg->setUniform("cameraDir", Vec3(camera.getTransfToGlobale()[2][0], camera.getTransfToGlobale()[2][1], camera.getTransfToGlobale()[2][2]));
	mShaderProg->setUniform("fov", camera.getFov());
	mShaderProg->setUniform("aspect", camera.getAspect());
	mShaderProg->setUniform("time", mTime);

	glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);

	//glDepthRange(0.f, 1.f);
	glEnable(GL_DEPTH_TEST);
}
