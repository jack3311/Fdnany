#pragma once

#include <memory>
#include <vector>

#include "Renderer.h"
#include "PerlinNoise.h"

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
	};

	class RendererTerrain : RendererInterface
	{
	private:
		Renderer<VertexFormatTerrain, true> rendererBase;
		std::unique_ptr<Maths::PerlinNoise> perlinNoise;

		void generateTerrainChunk(ivec2 _offsetXZ, ivec2 _scaleXZ, int _step, std::vector<VertexFormatTerrain> & _vertices, std::vector<GLuint> & _indices);

	public:
		RendererTerrain();
		~RendererTerrain();

		bool initialise();

		virtual void begin() const;
		virtual void draw() const;
		virtual void end() const;
	};
}