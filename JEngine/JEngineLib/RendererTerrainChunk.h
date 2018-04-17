#pragma once

#include "Renderer.h"

#include "VertexFormatTerrain.h"
#include "PerlinNoise.h"

namespace JEngine
{
	class RendererTerrainChunk : RendererInterface
	{
	private:
		ivec2 chunkPosXZ;
		ivec2 sizeXZ;
		float step;

		Renderer<VertexFormatTerrain, true> rendererBase;

	public:
		RendererTerrainChunk(ivec2 _offsetXZ, ivec2 _sizeXZ, float _step);
		~RendererTerrainChunk();

		void generateVertices(const Maths::PerlinNoise & _perlinNoise);

		bool initialise();

		virtual void begin() const;
		virtual void draw() const;
		virtual void end() const;
	};
}