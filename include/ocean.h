#ifndef OCEAN_H
#define OCEAN_H

#include <cstdint>

#include <GL/glew.h>

#include "app.h"
#include "drawable.h"
#include "shaderprogram.h"
#include "types.h"


class Ocean : public Drawable
{
public:
	Ocean();
	~Ocean();

	virtual void update(float elapsedTime) override;
	virtual void draw(const Camera &camera) const override;

private:
	GLuint vao, vbo;
	std::shared_ptr<const ShaderProgram> mProg;
	std::size_t mVertexCount;
	float mShift;
};

#endif // OCEAN_H
