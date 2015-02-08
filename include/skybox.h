#ifndef SKYBOX_H
#define SKYBOX_H

#include <GL/glew.h>

#include "drawable.h"
#include "filesystem.h"
#include "shaderprogram.h"
#include "types.h"


class SkyBox : public Drawable
{
public:
	SkyBox();
	~SkyBox();

	virtual void update(float elapsedTime);
	virtual void draw(const Camera &camera) const;
private:
	FileSystem mFS;
	shared_ptr<const ShaderProgram> mShaderProg;
	GLuint vao;
	GLuint vbo;

	float mTime;
};

#endif // TESTBOX_H
