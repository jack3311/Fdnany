#pragma once

#include "Maths.h"

namespace JEngine
{
	struct VertexFormatTerrain
	{
		vec3 position;
		vec3 normal;
		vec2 texCoords;
		vec4 texFactors;

		static void setupVertexAttributes();

		VertexFormatTerrain(const vec3 &, const vec3 &, const vec2 &, const vec4 &);

		bool operator<(const VertexFormatTerrain & b) const;
	};
}

