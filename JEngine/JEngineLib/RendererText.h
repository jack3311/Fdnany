#pragma once

#include "Renderer.h"

namespace JEngine
{
	class ResourceFont;
	
	struct VertexFormatText
	{
		vec2 position;
		vec2 texCoords;

		static void setupVertexAttributes();

		VertexFormatText(const vec2 &, const vec2 &);
	};

	class RendererText : protected Renderer<VertexFormatText, false>
	{
	public:
		RendererText();
		~RendererText();

		void draw(const ResourceFont & _font, const vec2 & _position, const float & _scale, const std::string & _text) const;
	
		bool initialise();
	};
}