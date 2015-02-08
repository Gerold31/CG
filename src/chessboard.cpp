#include "chessboard.h"

#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "filesystem.h"

#define CUBE_MIN_X (-1.f)
#define CUBE_MAX_X ( 1.f)
#define CUBE_MIN_Y (-.1f)
#define CUBE_MAX_Y ( .1f)
#define CUBE_MIN_Z (-1.f)
#define CUBE_MAX_Z ( 1.f)
#define CUBE_SIZE (6*6)
#define NUM_TILES_X (8)
#define NUM_TILES_Z (8)
#define NUM_VERTICES (NUM_TILES_X*NUM_TILES_Z*CUBE_SIZE)

ChessBoard::ChessBoard() :
	mFS("./"),
	mShaderProg(mFS.getShaderProgram("shader/wood.sp"))
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	GLfloat cube[CUBE_SIZE * 10] = {
		CUBE_MIN_X, CUBE_MIN_Y, CUBE_MIN_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		CUBE_MAX_X, CUBE_MIN_Y, CUBE_MIN_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		CUBE_MAX_X, CUBE_MAX_Y, CUBE_MIN_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		CUBE_MAX_X, CUBE_MAX_Y, CUBE_MIN_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		CUBE_MIN_X, CUBE_MAX_Y, CUBE_MIN_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		CUBE_MIN_X, CUBE_MIN_Y, CUBE_MIN_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,

		CUBE_MAX_X, CUBE_MIN_Y, CUBE_MIN_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		CUBE_MAX_X, CUBE_MIN_Y, CUBE_MAX_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		CUBE_MAX_X, CUBE_MAX_Y, CUBE_MAX_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		CUBE_MAX_X, CUBE_MAX_Y, CUBE_MAX_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		CUBE_MAX_X, CUBE_MAX_Y, CUBE_MIN_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		CUBE_MAX_X, CUBE_MIN_Y, CUBE_MIN_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,

		CUBE_MAX_X, CUBE_MIN_Y, CUBE_MAX_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		CUBE_MIN_X, CUBE_MIN_Y, CUBE_MAX_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		CUBE_MIN_X, CUBE_MAX_Y, CUBE_MAX_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		CUBE_MIN_X, CUBE_MAX_Y, CUBE_MAX_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		CUBE_MAX_X, CUBE_MAX_Y, CUBE_MAX_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		CUBE_MAX_X, CUBE_MIN_Y, CUBE_MAX_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,

		CUBE_MIN_X, CUBE_MIN_Y, CUBE_MAX_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		CUBE_MIN_X, CUBE_MIN_Y, CUBE_MIN_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		CUBE_MIN_X, CUBE_MAX_Y, CUBE_MIN_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		CUBE_MIN_X, CUBE_MAX_Y, CUBE_MIN_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		CUBE_MIN_X, CUBE_MAX_Y, CUBE_MAX_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		CUBE_MIN_X, CUBE_MIN_Y, CUBE_MAX_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,

		CUBE_MIN_X, CUBE_MIN_Y, CUBE_MIN_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		CUBE_MIN_X, CUBE_MIN_Y, CUBE_MAX_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		CUBE_MAX_X, CUBE_MIN_Y, CUBE_MAX_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		CUBE_MAX_X, CUBE_MIN_Y, CUBE_MAX_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		CUBE_MAX_X, CUBE_MIN_Y, CUBE_MIN_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		CUBE_MIN_X, CUBE_MIN_Y, CUBE_MIN_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,

		CUBE_MIN_X, CUBE_MAX_Y, CUBE_MAX_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		CUBE_MIN_X, CUBE_MAX_Y, CUBE_MIN_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		CUBE_MAX_X, CUBE_MAX_Y, CUBE_MIN_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		CUBE_MAX_X, CUBE_MAX_Y, CUBE_MIN_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		CUBE_MAX_X, CUBE_MAX_Y, CUBE_MAX_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		CUBE_MIN_X, CUBE_MAX_Y, CUBE_MAX_Z, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f
	};


	GLfloat *data = new GLfloat [NUM_VERTICES * 10];

	Vec3 dark = Vec3(150/256., 100/256., 50/256.);
	Vec3 light = Vec3(238/256., 197/256., 145/256.);
	float seed = rand()%(1<<16);

	for(int x=0; x<NUM_TILES_X; x++)
	{
		for(int z=0; z<NUM_TILES_Z; z++)
		{
			float posX = (x-NUM_TILES_X/2.) * (CUBE_MAX_X - CUBE_MIN_X);
			float posZ = (z-NUM_TILES_Z/2.) * (CUBE_MAX_Z - CUBE_MIN_Z);
			seed+=1.;
			for(int i=0; i<CUBE_SIZE; i++)
			{
				int idx = ((x*NUM_TILES_Z + z)*CUBE_SIZE + i)*10;

				data[idx + 0] = cube[i*10 + 0] + posX;
				data[idx + 1] = cube[i*10 + 1];
				data[idx + 2] = cube[i*10 + 2] + posZ;
				if((x+z)%2 == 0)
				{
					data[idx + 3] = dark.x;
					data[idx + 4] = dark.y;
					data[idx + 5] = dark.z;
				}else
				{
					data[idx + 3] = light.x;
					data[idx + 4] = light.y;
					data[idx + 5] = light.z;
				}
				data[idx + 6] = cube[i*10 + 6];
				data[idx + 7] = cube[i*10 + 7];
				data[idx + 8] = cube[i*10 + 8];
				data[idx + 9] = seed;
			}
		}
	}
	glBufferData(GL_ARRAY_BUFFER, NUM_VERTICES * 10 * sizeof(GLfloat), data, GL_STATIC_DRAW);

	mShaderProg->setVertexAttribPointer("pos", 3, GL_FLOAT, 10*sizeof(GLfloat), 0);
	mShaderProg->setVertexAttribPointer("color", 3, GL_FLOAT, 10*sizeof(GLfloat), 3*sizeof(GLfloat));
	mShaderProg->setVertexAttribPointer("normal", 3, GL_FLOAT, 10*sizeof(GLfloat), 6*sizeof(GLfloat));
	mShaderProg->setVertexAttribPointer("seed", 1, GL_FLOAT, 10*sizeof(GLfloat), 9*sizeof(GLfloat));
}

ChessBoard::~ChessBoard()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void ChessBoard::update(float elapsedTime)
{
	//rotate(M_PI_4*elapsedTime, Vec3(0.f, 1.f, 0.f));
}

void ChessBoard::draw(const Camera &camera) const
{
	mShaderProg->use();
	glBindVertexArray(vao);

	mShaderProg->setUniform("proj", camera.getProjection());
	mShaderProg->setUniform("model", getTransfToGlobale());

	glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);
}
