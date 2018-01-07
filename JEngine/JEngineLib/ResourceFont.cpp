#include "ResourceFont.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Logger.h"
#include "Util.h"

namespace JEngine
{
	FontCharacter::FontCharacter()
	{
	}

	FontCharacter::~FontCharacter()
	{
	}


	ResourceFont::ResourceFont(const std::string & _path) :
		path(_path), characters(NUM_CHARACTERS)
	{
	}

	ResourceFont::~ResourceFont()
	{
	}

	bool ResourceFont::initialise()
	{
		//FreeType
		FT_Library ft;

		//All functions return a value different than 0 whenever an error occurred
		ERR_IF(FT_Init_FreeType(&ft), "Could not initialise FreeType");

		//Load font as face
		FT_Face face;
		ERR_IF(FT_New_Face(ft, path.c_str(), 0, &face), "Could not load font");

		//Set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 48);

		//Disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		for (unsigned char c = 0; c < NUM_CHARACTERS; ++c)
		{
			//Load character
			ERR_IF(FT_Load_Char(face, c, FT_LOAD_RENDER), strJoinConvert("Could not load character: ", c));

			auto & character = characters[c];

			character.texture =
				std::make_unique<ResourceTexture>(
					face->glyph->bitmap.buffer,
					face->glyph->bitmap.width,
					face->glyph->bitmap.rows,
					ResourceTexture::ResourceTextureFormat::R);

			ERR_IF(!character.texture->initialise(), strJoinConvert("Could not load texture for character: ", c));

			character.size.set_xy(face->glyph->bitmap.width, face->glyph->bitmap.rows);
			character.bearing.set_xy(face->glyph->bitmap_left, face->glyph->bitmap_top);
			character.advance = static_cast<unsigned int>(face->glyph->advance.x);
		}

		//Clean up
		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		return true;
	}

}