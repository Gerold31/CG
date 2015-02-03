#ifndef TESTBOX_H
#define TESTBOX_H

#include <GL/glew.h>

#include "drawable.h"
#include "filesystem.h"
#include "shaderprogram.h"
#include "types.h"


class TestBox : public Drawable
{
public:
	TestBox();
	~TestBox();

	virtual void update(float elapsedTime);
	virtual void draw(const Camera &camera) const;
private:
	FileSystem mFS;
	shared_ptr<const ShaderProgram> mShaderProg;
	GLuint vao;
	GLuint vbo;
};

#endif // TESTBOX_H
