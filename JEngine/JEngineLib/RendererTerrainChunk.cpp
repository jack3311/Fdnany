#include "RendererTerrainChunk.h"

namespace JEngine
{
	RendererTerrainChunk::RendererTerrainChunk(ivec2 _chunkPosXZ, ivec2 _sizeXZ, float _step) :
		chunkPosXZ(_chunkPosXZ),
		sizeXZ(_sizeXZ),
		step(_step)
	{
	}

	RendererTerrainChunk::~RendererTerrainChunk()
	{
	}

	void RendererTerrainChunk::generateVertices(const Maths::PerlinNoise & _perlinNoise)
	{
		auto & _vertices = rendererBase.getVertices();
		auto & _indices = rendererBase.getIndices();

		int stepCountX = (sizeXZ.x / step) + 1; //Add 1 since the end edges of a chunk = the start edges of another
		int stepCountZ = (sizeXZ.y / step) + 1;

		//Generate vertices
		_vertices.resize(stepCountX * stepCountZ, VertexFormatTerrain{ { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0 }, { 0, 0, 0, 0 } });

		for (int x = 0; x < stepCountX; x += 1)
		{
			for (int z = 0; z < stepCountZ; z += 1)
			{
				float xpos = float(chunkPosXZ.x * sizeXZ.x) + float(x) * step;
				float zpos = float(chunkPosXZ.y * sizeXZ.y) + float(z) * step;

				float h1 = _perlinNoise.noise((float)xpos * 0.1f, (float)zpos * 0.1f, 0.1f);

				float ypos = h1;

				//For texture
				float hScaled = 3.f * h1; //3 = num textures - 1


				_vertices[x * stepCountZ + z] = VertexFormatTerrain{
					vec3{ xpos, ypos, zpos },
					vec3{ 0, 0, 0 },
					vec2{ x, z },
					vec4{
						MAX(1.f - fabsf(hScaled - 0.f), 0.f),
						MAX(1.f - fabsf(hScaled - 1.f), 0.f),
						MAX(1.f - fabsf(hScaled - 2.f), 0.f),
						MAX(1.f - fabsf(hScaled - 3.f), 0.f),
					}
				};
			}
		}


		//Calculate normals
		for (int x = 0; x < stepCountX - 1; ++x)
		{
			for (int z = 0; z < stepCountZ - 1; ++z)
			{
				glm::vec3 pos0 = _vertices[x * stepCountZ + z].position;
				glm::vec3 pos1 = _vertices[x * stepCountZ + z + 1].position;
				glm::vec3 pos2 = _vertices[(x + 1) * stepCountZ + z].position;

				auto del0 = pos1 - pos0;
				auto del1 = pos2 - pos0;
				auto nor = glm::cross(del0, del1);

				_vertices[x * stepCountZ + z].normal.x = nor.x;
				_vertices[x * stepCountZ + z].normal.y = nor.y;
				_vertices[x * stepCountZ + z].normal.z = nor.z;
			}
		}

		//Calculate indices
		_indices.resize((stepCountX - 1) * (stepCountZ - 1) * 6);

		for (int x = 0; x < stepCountX - 1; ++x)
		{
			for (int z = 0; z < stepCountZ - 1; ++z)
			{
				int startIndex = x * (stepCountZ - 1) * 6 + z * 6;

				_indices[startIndex] = (x * stepCountZ + z);					//0
				_indices[startIndex + 1] = (x * stepCountZ + z + 1);			//1		
				_indices[startIndex + 2] = ((x + 1) * stepCountZ + (z + 1));	//3

				_indices[startIndex + 3] = (x * stepCountZ + z);				//0
				_indices[startIndex + 4] = ((x + 1) * stepCountZ + (z + 1));	//3
				_indices[startIndex + 5] = ((x + 1) * stepCountZ + z);		//2
			}
		}
	}

	bool RendererTerrainChunk::initialise()
	{
		return rendererBase.initialise();
	}

	void RendererTerrainChunk::begin() const
	{
		rendererBase.begin();
	}
	void RendererTerrainChunk::draw() const
	{
		rendererBase.draw();
	}
	void RendererTerrainChunk::end() const
	{
		rendererBase.end();
	}
}


