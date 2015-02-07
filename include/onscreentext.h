#ifndef ONSCREENTEXT_H
#define ONSCREENTEXT_H

#include <cstdint>
#include <memory>
#include <string>

#include "drawable.h"
#include "fontface.h"
#include "shaderprogram.h"


class OnScreenText : public Drawable
{
public:
	OnScreenText(const std::string &text);
	~OnScreenText();

	virtual void draw(const Camera &camera) const override;
	virtual void update(float elapsedTime) override;

	void setText(const std::string &text);
private:
	FontFace mFace;
	std::uint16_t mSize;
	std::string mText;

	GLuint vao, vbo;
	std::shared_ptr<const ShaderProgram> mProg;
};


inline void OnScreenText::setText(const std::string &text)
{
	mText = text;
}

#endif // ONSCREENTEXT_H
