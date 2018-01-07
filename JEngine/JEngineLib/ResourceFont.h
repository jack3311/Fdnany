#pragma once

#include <vector>
#include <memory>

#include "ResourceManagement.h"
#include "Maths.h"

namespace JEngine
{
#define NUM_CHARACTERS 128

	class FontCharacter
	{
	public:
		std::unique_ptr<ResourceTexture> texture; //Texture
		ivec2 size; //Size of glyph
		ivec2 bearing; //Offset from baseline to left/top of glyph
		unsigned int advance; // Offset to advance to next glyph

		FontCharacter();
		~FontCharacter();
	};

	class ResourceFont :
		public Resource
	{
	private:
		std::vector<FontCharacter> characters;
		std::string path;

	public:
		ResourceFont(const std::string & _path);
		virtual ~ResourceFont();

		bool initialise();

		void draw(const fvec2 & _position, const float & _scale, const std::string & _text);
	};
}