#include "RendererTerrain.h"

#include "Util.h"
#include "Logger.h"

namespace JEngine
{
	RendererTerrain::RendererTerrain()
	{
	}
	RendererTerrain::~RendererTerrain()
	{
	}

	bool RendererTerrain::initialise()
	{
		perlinNoise = std::make_unique<Maths::PerlinNoise>(10);

		for (int cx = -10; cx < 10; ++cx)
		{
			for (int cz = -10; cz < 10; ++cz)
			{
				ivec2 chunkPos{ cx, cz };

				int chunkPosID = (chunkPos.x << 8) + chunkPos.y;

				chunks[chunkPosID] = std::make_unique<RendererTerrainChunk>(chunkPos, ivec2{ 9, 9 }, 3.f);

				chunks[chunkPosID]->generateVertices(*perlinNoise);

				ERR_IF(!chunks[chunkPosID]->initialise(), "Could not initialise terrain chunk", "Initialised terrain chunk");
			}
		}

		return true;
	}


	void RendererTerrain::begin() const
	{
	}

	void RendererTerrain::draw() const
	{
		for (auto itr = chunks.begin(); itr != chunks.end(); ++itr)
		{
			itr->second->begin();
			itr->second->draw();
			itr->second->end();
		}
	}

	void RendererTerrain::end() const
	{
	}
}
