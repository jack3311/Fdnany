#include "RendererText.h"

#include "ResourceFont.h"


namespace JEngine
{
	void VertexFormatText::setupVertexAttributes()
	{
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormatText), (GLvoid *)offsetof(VertexFormatText, VertexFormatText::position));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormatText), (GLvoid *)offsetof(VertexFormatText, VertexFormatText::texCoords));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
	}

	VertexFormatText::VertexFormatText(vec2 _position, vec2 _texCoords) :
		position(_position), texCoords(_texCoords)
	{
	}


	RendererText::RendererText() :
		Renderer(std::vector<VertexFormatText>(6u, VertexFormatText{ vec2(), vec2() }))
	{
	}

	RendererText::~RendererText()
	{
	}

	void RendererText::draw(const ResourceFont & _font, const fvec2 & _position, const float & _scale, const std::string & _text) const
	{
		assert(Engine::getEngine().isCurrentThreadMain());

		EnableCullFace;
		EnableBlend;

		glActiveTexture(GL_TEXTURE0);

		auto & characters = _font.getCharacters();

		for (auto cCurrent = _text.begin(); cCurrent != _text.end(); ++cCurrent)
		{
			const FontCharacter & fc = characters[*cCurrent];

			float	x = _position.x + fc.bearing.x * _scale,
					y = _position.y - (fc.size.y - fc.bearing.y) * _scale;

			float	width = fc.size.x * _scale,
					height = fc.size.y * _scale;

			//Update VBO with this character's vertices
			std::vector<VertexFormatText> verticesNew{
				VertexFormatText{ vec2{ x, y + height },			vec2{ 0.f, 0.f } }, //0
				VertexFormatText{ vec2{ x + width, y + height },	vec2{ 1.f, 0.f } }, //1
				VertexFormatText{ vec2{ x, y },						vec2{ 0.f, 1.f } }, //2

				VertexFormatText{ vec2{ x + width, y + height },	vec2{ 1.f, 0.f } }, //1
				VertexFormatText{ vec2{ x + width, y },				vec2{ 1.f, 1.f } }, //3
				VertexFormatText{ vec2{ x, y },						vec2{ 0.f, 1.f } }  //2
			};
		}
	}
}