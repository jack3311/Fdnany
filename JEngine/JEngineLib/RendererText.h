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

		VertexFormatText(vec2, vec2);
	};

	class RendererText : public Renderer<VertexFormatText, false>
	{
	private:
		//Force renderer draw to be private
		using Renderer::draw;

	public:
		RendererText();
		~RendererText();

		void draw(const ResourceFont & _font, const vec2 & _position, const float & _scale, const std::string & _text, const glm::mat4 & _model = glm::mat4()) const;
	};
}