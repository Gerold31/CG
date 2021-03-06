#include "ocean.h"

#include <cmath>
#include <vector>

#include "camera.h"
#include "scene.h"
#include "light.h"

#define OCEAN_VIEW_DIST 100
#define OCEAN_HIGHEST_ACCURACY 0.125

struct vertex {
	GLfloat x, z;
};

Ocean::Ocean() :
	mProg(APP->getFileSystem()->getShaderProgram("shader/ocean.sp")),
	mShift(0.f)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	std::vector<vertex> vertices;

//	vertices.push_back({-OCEAN_HIGHEST_ACCURACY, OCEAN_HIGHEST_ACCURACY});
//	vertices.push_back({-OCEAN_HIGHEST_ACCURACY,                    0.f});
//	vertices.push_back({                    0.f, OCEAN_HIGHEST_ACCURACY}); //L
//	vertices.push_back({                    0.f,                    0.f}); //R
//	vertices.push_back({ OCEAN_HIGHEST_ACCURACY, OCEAN_HIGHEST_ACCURACY}); //L
//	vertices.push_back({ OCEAN_HIGHEST_ACCURACY,                    0.f}); //R
//	vertices.push_back({ OCEAN_HIGHEST_ACCURACY,-OCEAN_HIGHEST_ACCURACY}); //L
//	vertices.push_back({                    0.f,                    0.f}); //R
//	vertices.push_back({                    0.f,-OCEAN_HIGHEST_ACCURACY}); //L
//	vertices.push_back({-OCEAN_HIGHEST_ACCURACY,                    0.f}); //R
//	vertices.push_back({-OCEAN_HIGHEST_ACCURACY,-OCEAN_HIGHEST_ACCURACY}); //L
//	for (float accuracy = OCEAN_HIGHEST_ACCURACY; accuracy < OCEAN_VIEW_DIST; accuracy *= 3) {
//		vertices.push_back({-accuracy             ,-accuracy - 2*accuracy}); //R
//		vertices.push_back({ accuracy             ,-accuracy             }); //L
//		vertices.push_back({ accuracy             ,-accuracy - 2*accuracy}); //R
//		vertices.push_back({ accuracy + 2*accuracy,-accuracy             }); //L
//		vertices.push_back({ accuracy + 2*accuracy,-accuracy - 2*accuracy}); //R
//		vertices.push_back({ accuracy + 2*accuracy,-accuracy             }); //L
//		vertices.push_back({ accuracy + 2*accuracy,-accuracy             }); //R
//		vertices.push_back({ accuracy             ,-accuracy             }); //L
//		vertices.push_back({ accuracy + 2*accuracy, accuracy             }); //R
//		vertices.push_back({ accuracy             , accuracy             }); //L
//		vertices.push_back({ accuracy + 2*accuracy, accuracy + 2*accuracy}); //R
//		vertices.push_back({ accuracy             , accuracy + 2*accuracy}); //L
//		vertices.push_back({ accuracy             , accuracy + 2*accuracy}); //R
//		vertices.push_back({ accuracy             , accuracy             }); //L
//		vertices.push_back({-accuracy             , accuracy + 2*accuracy}); //R
//		vertices.push_back({-accuracy             , accuracy             }); //L
//		vertices.push_back({-accuracy - 2*accuracy, accuracy + 2*accuracy}); //R
//		vertices.push_back({-accuracy - 2*accuracy, accuracy             }); //L
//		vertices.push_back({-accuracy - 2*accuracy, accuracy             }); //R
//		vertices.push_back({-accuracy             , accuracy             }); //L
//		vertices.push_back({-accuracy - 2*accuracy,-accuracy             }); //R
//		vertices.push_back({-accuracy             ,-accuracy             }); //L
//		vertices.push_back({-accuracy - 2*accuracy,-accuracy - 2*accuracy}); //R
//		vertices.push_back({-accuracy             ,-accuracy - 2*accuracy}); //L
//		vertices.push_back({-accuracy - 2*accuracy,-accuracy - 2*accuracy}); //R
//		vertices.push_back({-accuracy - 2*accuracy,-accuracy - 2*accuracy}); //L
//	}

	const float acc = OCEAN_HIGHEST_ACCURACY;
	for (float x = OCEAN_VIEW_DIST - 1; x > -OCEAN_VIEW_DIST; x -= 2 * acc) {
		for (float z = -OCEAN_VIEW_DIST; z <= OCEAN_VIEW_DIST; z += acc) {
			vertices.push_back({(GLfloat) x + acc, (GLfloat) z});
			vertices.push_back({(GLfloat) x,       (GLfloat) z});
		}
		for (float z = OCEAN_VIEW_DIST; z > -OCEAN_VIEW_DIST; z -= acc) {
			vertices.push_back({(GLfloat) x - acc, (GLfloat) z      });
			vertices.push_back({(GLfloat) x,       (GLfloat) z - acc});
		}
	}

	mVertexCount = vertices.size();
	glBufferData(GL_ARRAY_BUFFER, mVertexCount * sizeof(vertex), vertices.data(), GL_STATIC_DRAW);
	mProg->setVertexAttribPointer("poscoord", 2, GL_FLOAT, sizeof(vertex), 0);
}

Ocean::~Ocean()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void Ocean::update(float elapsedTime)
{
	static const float interval = 64000;
	mShift += elapsedTime;
	mShift -= interval * std::floor(mShift / interval);
}

void Ocean::draw(const Camera &camera) const
{
	mProg->use();
	mProg->setUniform("campos", (Vec3)camera.getPosition());
	mProg->setUniform("proj", camera.getProjection());
	mProg->setUniform("shift", mShift);
	mProg->setUniform("height", getPosition().y);

	//lighting
	mProg->setUniform("ambientLight", getScene()->getAmbientLight());

	size_t i = getScene()->getNumLights();
	mProg->setUniform("numLights", (GLint) i);
	for(size_t i=0; i<getScene()->getNumLights(); i++)
	{
		shared_ptr<Light> l = getScene()->getLight(i);

		mProg->setUniform("lights", i, "position", l->getPosition());
		mProg->setUniform("lights", i, "color", l->getColor());
		mProg->setUniform("lights", i, "attenuation", l->getAttenuation());
	}

	glBindVertexArray(vao);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, mVertexCount);
}
