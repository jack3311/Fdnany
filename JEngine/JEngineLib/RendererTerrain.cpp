#include "RendererTerrain.h"

#include "Util.h"
#include "Logger.h"

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


	void RendererTerrain::generateTerrainChunk(ivec2 _offsetXZ, ivec2 _sizeXZ, int _step, std::vector<VertexFormatTerrain> & _vertices, std::vector<GLuint> & _indices)
	{
		//Generate vertices
		_vertices.resize(_sizeXZ.x * _sizeXZ.y, VertexFormatTerrain{ {0, 0, 0}, {0, 0, 0}, {0, 0}, {0, 0, 0, 0} });

		for (int x = 0; x < _sizeXZ.x; x += _step)
		{
			for (int z = 0; z < _sizeXZ.y; z += _step)
			{
				float xpos = (float)(_offsetXZ.x + _step * x);
				float zpos = (float)(_offsetXZ.y + _step * z);

				float h1 = perlinNoise->noise((float)xpos * 0.1f, (float)zpos * 0.1f, 0.1f);

				float ypos = h1;

				//For texture
				float hScaled = 3.f * h1; //3 = num textures - 1


				_vertices[x * _sizeXZ.y + z] = VertexFormatTerrain{ 
					vec3{xpos, ypos, zpos}, 
					vec3{0, 0, 0}, 
					vec2{x, z}, 
					vec4{
						MAX(1.f - fabsf(hScaled - 0.f), 0.f),
						MAX(1.f - fabsf(hScaled - 1.f), 0.f),
						MAX(1.f - fabsf(hScaled - 2.f), 0.f),
						MAX(1.f - fabsf(hScaled - 3.f), 0.f),
					} 
				};
			}
		}

		std::vector<VertexFormatTerrain> verticesSorted;
		verticesSorted.insert(verticesSorted.begin(), _vertices.begin(), _vertices.end());
		std::sort(verticesSorted.begin(), verticesSorted.end());
		/*std::sort(verticesSorted.begin(), verticesSorted.end(), [](const VertexFormatTerrain & a, const VertexFormatTerrain & b) {
			return a.position.y < b.position.y ? a : b;
		});*/

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

		//Calculate indices
		_indices.resize((_sizeXZ.x - 1) * (_sizeXZ.y - 1) * 6);

		for (int x = 0; x < _sizeXZ.x - 1; ++x)
		{
			for (int z = 0; z < _sizeXZ.y - 1; ++z)
			{
				int startIndex = x * (_sizeXZ.y - 1) * 6 + z * 6;

				_indices[startIndex] = (x * _sizeXZ.y + z);					//0
				_indices[startIndex + 1] = (x * _sizeXZ.y + z + 1);			//1		
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

		std::vector<VertexFormatTerrain> vertices;
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
