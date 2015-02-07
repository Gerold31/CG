#include "onscreentext.h"

#include "app.h"
#include "filesystem.h"
#include "fontface.h"
#include "logging.h"


OnScreenText::OnScreenText(const std::string &text) :
	mFace(APP->getFileSystem()->getFullPath("fonts/FreeSans.ttf")),
	mSize(16),
	mText(text),
	mProg(APP->getFileSystem()->getShaderProgram("shader/onscreentext.sp"))
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	mProg->setVertexAttribPointer("poscoord", 2, GL_FLOAT, 4*sizeof(GLfloat), 0);
	mProg->setVertexAttribPointer("texcoord", 2, GL_FLOAT, 4*sizeof(GLfloat), 2*sizeof(GLfloat));
}

OnScreenText::~OnScreenText()
{
}

void OnScreenText::draw(const Camera &camera) const
{
	float x = getPosition()[0];
	float y = getPosition()[1];
	mProg->use();
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	for (char c : mText) {
		const FontFace::Glyph &glyph = mFace.getGlyph(c, mSize);
		float sx = 2.f / APP->getResolution().x;
		float sy = 2.f / APP->getResolution().y;
		float x2 = x + glyph.left * sx;
		float y2 = - y - glyph.top * sy;
		float w = glyph.width * sx;
		float h = glyph.height * sy;
		float maxX = (float) glyph.width / glyph.texWidth;
		float maxY = (float) glyph.height / glyph.texHeight;

		GLfloat buf[] = {
			x2,     -y2,     0.f,  0.f,
			x2 + w, -y2,     maxX, 0.f,
			x2,     -y2 - h, 0.f,  maxY,
			x2 + w, -y2 - h, maxX, maxY
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(buf), buf, GL_DYNAMIC_DRAW);
		glyph.texture->bindToSlot(1);
		//mProg->setUniform("glyph", 1);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		x += (glyph.advanceX >> 6) * sx;
		y += (glyph.advanceY >> 6) * sy;
	}

	glEnable(GL_DEPTH_TEST);
}

void OnScreenText::update(float elapsedTime)
{
	// Nothing to do
}
