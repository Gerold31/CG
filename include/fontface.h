#ifndef FONTFACE_H
#define FONTFACE_H

#include <cassert>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "filesystem.h"
#include "texture.h"


class FontFace
{
public:
	struct Glyph {
		std::uint16_t width;
		std::uint16_t height;
		std::uint16_t left;
		std::uint16_t top;
		std::uint16_t advanceX;
		std::uint16_t advanceY;
		std::uint16_t texWidth;
		std::uint16_t texHeight;
		std::shared_ptr<const Texture> texture;
	};

	FontFace(const std::string &name);
	~FontFace();

	const Glyph &getGlyph(char c, std::uint16_t size) const;
	const std::shared_ptr<const Texture> &getTexture(char c, std::uint16_t size) const;

private:
	FontFace(const FontFace &); // = delete;
	FontFace &operator=(const FontFace &); // = delete;

	void loadGlyph(Glyph &glyph, char c, std::uint16_t size) const;

	FT_Face face;
	mutable std::unordered_map<std::uint32_t, Glyph> mGlyphCache;
};


inline const FontFace::Glyph &FontFace::getGlyph(char c, std::uint16_t size) const
{
	Glyph &glyph = mGlyphCache[(c << 16) + size];
	if (glyph.texture == nullptr) {
		loadGlyph(glyph, c, size);
	}
	return glyph;
}

inline const std::shared_ptr<const Texture> &FontFace::getTexture(char c, std::uint16_t size) const
{
	return getGlyph(c, size).texture;
}

#endif // FONTFACE_H
