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

	VertexFormatText::VertexFormatText(vec2 _position, vec2 _texCoords) :
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
		assert(Engine::getEngine().isCurrentThreadMain());

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
			//std::vector<VertexFormatText> verticesNew{
			//	VertexFormatText{ vec2{ x, y + height },			vec2{ 0.f, 0.f } }, //0
			//	VertexFormatText{ vec2{ x + width, y + height },	vec2{ 1.f, 0.f } }, //1
			//	VertexFormatText{ vec2{ x, y },						vec2{ 0.f, 1.f } }, //2
			//
			//	VertexFormatText{ vec2{ x + width, y + height },	vec2{ 1.f, 0.f } }, //1
			//	VertexFormatText{ vec2{ x + width, y },				vec2{ 1.f, 1.f } }, //3
			//	VertexFormatText{ vec2{ x, y },						vec2{ 0.f, 1.f } }  //2
			//};

			GLfloat vertices1[6][4] = {
				{ x, y - height,				0.0, 0.0 },
				{ x + width, y - height,		1.0, 0.0 },
				{ x, y + height,				0.0, 1.0 },

				{ x + width, y - height,		1.0, 0.0 },
				{ x + width, y + height,		1.0, 1.0 },
				{ x, y + height,				0.0, 1.0 }
			};

			std::vector<VertexFormatText> verticesNew{
				{ {x, y - height},				{0.0, 0.0} },
				{ {x + width, y - height},		{1.0, 0.0} },
				{ {x, y + height},				{0.0, 1.0} },
	
				{ {x + width, y - height},		{1.0, 0.0} },
				{ {x + width, y + height},		{1.0, 1.0} },
				{ {x, y + height},				{0.0, 1.0} }
			};
			
			/*GLfloat vertices1[6][4] = {
				{ x, y + height,		0.0, 0.0 },
				{ x, y,			0.0, 1.0 },
				{ x + width, y,		1.0, 1.0 },

				{ x, y + height,		0.0, 0.0 },
				{ x + width, y,		1.0, 1.0 },
				{ x + width, y + height,	1.0, 0.0 }
			};*/

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, verticesNew.size() * sizeof(VertexFormatText), &verticesNew[0]); //TODO: can be glBufferData?
			//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices1), vertices1); // Be sure to use glBufferSubData and not glBufferData

			fc.texture->bind(GL_TEXTURE0);
			
			//Actually draw
			draw(_model);

			//Move forwards for next char
			textPosition.x += (fc.advance >> 6) * _scale;
		}
	}
}