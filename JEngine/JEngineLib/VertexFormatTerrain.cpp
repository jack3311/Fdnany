#include "VertexFormatTerrain.h"

#include <GL\glew.h>

namespace JEngine
{
	VertexFormatTerrain::VertexFormatTerrain(const vec3 & _position, const vec3 & _normal, const vec2 & _texCoords, const vec4 & _texFactors) :
		position(_position),
		normal(_normal),
		texCoords(_texCoords),
		texFactors(_texFactors)
	{
	}

	bool VertexFormatTerrain::operator<(const VertexFormatTerrain & b) const
	{
		return b.position.y < position.y;
	}

	void VertexFormatTerrain::setupVertexAttributes()
	{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormatTerrain), (GLvoid *)offsetof(VertexFormatTerrain, VertexFormatTerrain::position));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormatTerrain), (GLvoid *)offsetof(VertexFormatTerrain, VertexFormatTerrain::normal));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormatTerrain), (GLvoid *)offsetof(VertexFormatTerrain, VertexFormatTerrain::texCoords));
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(VertexFormatTerrain), (GLvoid *)offsetof(VertexFormatTerrain, VertexFormatTerrain::texFactors));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
	}
}

