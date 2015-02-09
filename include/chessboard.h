#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <GL/glew.h>

#include "drawable.h"
#include "filesystem.h"
#include "shaderprogram.h"
#include "types.h"


class ChessBoard : public Drawable
{
public:
	ChessBoard();
	~ChessBoard();

	virtual void update(float elapsedTime);
	virtual void draw(const Camera &camera) const;
private:
	FileSystem mFS;
	shared_ptr<const ShaderProgram> mShaderProg;
	GLuint vao;
	GLuint vbo;
};

#endif // TESTBOX_H
