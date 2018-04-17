#pragma once

#include <memory>
#include <vector>
#include <map>

#include "Renderer.h"
#include "RendererTerrainChunk.h"
#include "VertexFormatTerrain.h"

namespace JEngine
{

	class RendererTerrain : RendererInterface
	{
	private:
		std::map<int, std::unique_ptr<RendererTerrainChunk>> chunks;

		std::unique_ptr<Maths::PerlinNoise> perlinNoise;

	public:
		RendererTerrain();
		~RendererTerrain();

		bool initialise();

		virtual void begin() const;
		virtual void draw() const;
		virtual void end() const;
	};
}