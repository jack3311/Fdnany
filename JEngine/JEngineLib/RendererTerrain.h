#pragma once

#include <memory>
#include <vector>

#include "Renderer.h"
#include "PerlinNoise.h"

namespace JEngine
{
	class RendererTerrain : RendererInterface
	{
	private:
		Renderer<VertexFormatStandard, true> rendererBase;
		std::unique_ptr<Maths::PerlinNoise> perlinNoise;

		void generateTerrainChunk(ivec2 _offsetXZ, ivec2 _scaleXZ, int _step, std::vector<VertexFormatStandard> & _vertices, std::vector<GLuint> & _indices);

	public:
		RendererTerrain();
		~RendererTerrain();

		bool initialise();

		virtual void begin() const;
		virtual void draw() const;
		virtual void end() const;
	};
}