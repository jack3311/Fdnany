#include "RendererTerrain.h"

#include "Util.h"
#include "Logger.h"

namespace JEngine
{
	void RendererTerrain::generateTerrainChunk(ivec2 _offsetXZ, ivec2 _sizeXZ, int _step, std::vector<VertexFormatStandard> & _vertices, std::vector<GLuint> & _indices)
	{
		_vertices.resize(_sizeXZ.x * _sizeXZ.y, VertexFormatStandard{ {0, 0, 0}, {0, 0, 0}, {0, 0} });

		for (int x = 0; x < _sizeXZ.x; x += _step)
		{
			for (int z = 0; z < _sizeXZ.y; z += _step)
			{
				float xpos = _offsetXZ.x + _step * x;
				float zpos = _offsetXZ.y + _step * z;
				float ypos = perlinNoise->octaveNoise((float)xpos * 0.1f, (float)zpos * 0.1f, 0.1f);

				_vertices[x * _sizeXZ.y + z] = VertexFormatStandard{ vec3{xpos, ypos, zpos}, vec3{0, 0, 0}, vec2{x, z} };
			}
		}

		//Calculate normals
		for (int x = 0; x < _sizeXZ.x - 1; ++x)
		{
			for (int z = 0; z < _sizeXZ.y - 1; ++z)
			{
				glm::vec3 pos0 = _vertices[x * _sizeXZ.y + z].position;
				glm::vec3 pos1 = _vertices[x * _sizeXZ.y + z + 1].position;
				glm::vec3 pos2 = _vertices[(x + 1) * _sizeXZ.y + z].position;

				auto del0 = pos1 - pos0;
				auto del1 = pos2 - pos0;
				auto nor = glm::cross(del0, del1);

				_vertices[x * _sizeXZ.y + z].normal.x = nor.x;
				_vertices[x * _sizeXZ.y + z].normal.y = nor.y;
				_vertices[x * _sizeXZ.y + z].normal.z = nor.z;
			}
		}

		_indices.resize((_sizeXZ.x - 1) * (_sizeXZ.y - 1) * 6);

		//Add indices
		for (int x = 0; x < _sizeXZ.x - 1; ++x)
		{
			for (int z = 0; z < _sizeXZ.y - 1; ++z)
			{
				int startIndex = x * (_sizeXZ.y - 1) * 6 + z * 6;

				_indices[startIndex] = (x * _sizeXZ.y + z);				//0
				_indices[startIndex + 1] = (x * _sizeXZ.y + z + 1);		//1		
				_indices[startIndex + 2] = ((x + 1) * _sizeXZ.y + (z + 1));	//3
				
				_indices[startIndex + 3] = (x * _sizeXZ.y + z);				//0
				_indices[startIndex + 4] = ((x + 1) * _sizeXZ.y + (z + 1));	//3
				_indices[startIndex + 5] = ((x + 1) * _sizeXZ.y + z);		//2
			}
		}
	}

	RendererTerrain::RendererTerrain()
	{
	}
	RendererTerrain::~RendererTerrain()
	{
	}

	bool RendererTerrain::initialise()
	{
		perlinNoise = std::make_unique<Maths::PerlinNoise>(10);

		std::vector<VertexFormatStandard> vertices;
		std::vector<GLuint> indices;
		generateTerrainChunk({ 0, 0 }, { 100, 100 }, 1, vertices, indices);
		
		ERR_IF(!rendererBase.initialise(vertices, indices), "Could not initialise terrain renderer base", "Initialised terrain renderer base");

		return true;
	}
	void RendererTerrain::begin() const
	{
	}
	void RendererTerrain::draw() const
	{
		rendererBase.begin();
		rendererBase.draw();
		rendererBase.end();
	}
	void RendererTerrain::end() const
	{
	}
}
