#include "RendererText.h"

#include "ResourceFont.h"

#include "Engine.h"


namespace JEngine
{
	void VertexFormatText::setupVertexAttributes()
	{
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormatText), (GLvoid *)offsetof(VertexFormatText, VertexFormatText::position));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormatText), (GLvoid *)offsetof(VertexFormatText, VertexFormatText::texCoords));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
	}

	VertexFormatText::VertexFormatText(const vec2 & _position, const vec2 & _texCoords) :
		position(_position), texCoords(_texCoords)
	{
	}


	RendererText::RendererText() :
		Renderer(std::vector<VertexFormatText>(6u, VertexFormatText{ vec2(), vec2() }), GL_TRIANGLES, true)
	{
	}

	RendererText::~RendererText()
	{
	}

	void RendererText::draw(const ResourceFont & _font, const vec2 & _position, const float & _scale, const std::string & _text, const glm::mat4 & _model) const
	{
		assert(Engine::get().isCurrentThreadMain());

		EnableBlend;

		vec2 textPosition = _position;
		auto & characters = _font.getCharacters();

		for (auto cCurrent = _text.begin(); cCurrent != _text.end(); ++cCurrent)
		{
			const FontCharacter & fc = characters[*cCurrent];

			float	x = textPosition.x + fc.bearing.x * _scale,
					y = textPosition.y - (fc.size.y - fc.bearing.y) * _scale;

			float	width = fc.size.x * _scale,
					height = fc.size.y * _scale;

			//Update VBO with this character's vertices
			std::vector<VertexFormatText> verticesNew{
				{ {x, y},					{0.0f, 1.0f} }, //0
				{ {x + width, y},			{1.0f, 1.0f} }, //1
				{ {x, y + height},			{0.0f, 0.0f} }, //2

				{ {x + width, y},			{1.0f, 1.0f} }, //1
				{ {x + width, y + height},	{1.0f, 0.0f} }, //2
				{ {x, y + height},			{0.0f, 0.0f} } //3
			};

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, verticesNew.size() * sizeof(VertexFormatText), &verticesNew[0]); //TODO: can be glBufferData?
			
			fc.texture->bind(GL_TEXTURE0);
			
			//Actually draw
			draw(_model);

			//Move forwards for next char
			textPosition.x += (fc.advance >> 6) * _scale;
		}
	}
}