#include "fontface.h"

#include <cstdint>
#include <mutex>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "logging.h"
#include "texture.h"

static std::uint16_t toPowerOfTo(std::uint16_t n);

static FT_Library ftlib;
static std::once_flag initOFlag;
static void initFreeType() {
	if (FT_Init_FreeType(&ftlib)) {
		SEVERE("Could not initialize FreeType.");
		exit(EXIT_FAILURE); // TODO throw exception
	}
}

FontFace::FontFace(const std::string &name)
{
	std::call_once(initOFlag, initFreeType);
	if (FT_New_Face(ftlib, name.c_str(), 0, &face)) {
		SEVERE("Could not open font: %s", name.c_str());
		exit(EXIT_FAILURE); // TODO throw exception
	}
}

FontFace::~FontFace()
{
	FT_Done_Face(face);
}

void FontFace::loadGlyph(FontFace::Glyph &glyph, char c, std::uint16_t size) const
{
	// Load char with FreeType
	FT_Set_Pixel_Sizes(face, 0, size);
	if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
		WARNING("Could not load char: %c", c);
		return; // TODO
	}
	// Get data from FreeType
	glyph.width = face->glyph->bitmap.width;
	glyph.height = face->glyph->bitmap.rows;
	glyph.texWidth = toPowerOfTo(glyph.width);
	glyph.texHeight = toPowerOfTo(glyph.height);
	glyph.left = face->glyph->bitmap_left;
	glyph.top = face->glyph->bitmap_top;
	glyph.advanceX = face->glyph->advance.x;
	glyph.advanceY = face->glyph->advance.y;
	// Logging
	FINE("Create texture for '%c' with size %d. Texture size: [%d,%d]",
		c, size, glyph.texWidth, glyph.texHeight);
	// Create bitmap for texture
	GLubyte buffer[glyph.texHeight * glyph.texWidth];
	for (std::uint16_t x = 0; x < glyph.texWidth; ++x) {
		for (std::uint16_t y = 0; y < glyph.texHeight; ++y) {
			GLubyte value;
			if (x < glyph.width && y < glyph.height)
				value = face->glyph->bitmap.buffer[y * glyph.width + x];
			else
				value = 0;
			buffer[y * glyph.texWidth + x] = value;
		}
	}
	// Create texture
	std::shared_ptr<Texture> tex = std::make_shared<Texture>(GL_TEXTURE_2D);
	tex->setTexParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	tex->setTexParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	tex->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	tex->setTexParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	tex->setTexImage2D(GL_RED, glyph.texWidth, glyph.texHeight, GL_RED, GL_UNSIGNED_BYTE, buffer);
	glyph.texture = std::move(tex);
}


std::uint16_t toPowerOfTo(std::uint16_t n)
{
	std::uint16_t r = 4;
	while (r < n) {
		assert(r != 0);
		r <<= 1;
	}
	return r;
}
