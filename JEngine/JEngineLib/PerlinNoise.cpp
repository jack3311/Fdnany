#include "PerlinNoise.h"

#include <iostream>
#include <algorithm>


namespace JEngine
{
	namespace Maths
	{
		PerlinNoise::PerlinNoise(unsigned int seed) : gen(seed)
		{
			//Fill hashing array with 1-255
			randoms.resize(256);
			for (int i = 0; i < 256; ++i)
			{
				randoms[i] = i;
			}

			//Shuffle the array
			std::shuffle(randoms.begin(), randoms.end(), gen);

			//Duplicate it for 256 values total
			randoms.insert(randoms.end(), randoms.begin(), randoms.end());
		}

		float PerlinNoise::octaveNoise(float _x, float _y, float _z) const
		{
			static int octaves = 8;
			float persistance = 0.5f;
			float total = 0.f;

			for (int i = 0; i < octaves; ++i)
			{
				float freq = powf(2, static_cast<float>(i));
				float amplitude = powf(persistance, static_cast<float>(i));

				total += noise(_x * freq, _y * freq, _z * freq) * amplitude;
			}

			return total;
		}

		float PerlinNoise::noise(float _x, float _y, float _z) const
		{
			//Find the cube that we are in - limit to 255 because of hashing array
			int cubeX = static_cast<int>(floor(_x)) & 255;
			int cubeY = static_cast<int>(floor(_y)) & 255;
			int cubeZ = static_cast<int>(floor(_z)) & 255;

			//How far are we to next coords on each axis
			float lerptX = _x - floor(_x);
			float lerptY = _y - floor(_y);
			float lerptZ = _z - floor(_z);

			//Find hashes of each 8 cube coordinates - won't access error as each randoms[x] is 0-255
			int aaa = randoms[randoms[randoms[cubeX] + cubeY] + cubeZ],
				aba = randoms[randoms[randoms[cubeX] + cubeY + 1] + cubeZ],
				aab = randoms[randoms[randoms[cubeX] + cubeY] + cubeZ + 1],
				abb = randoms[randoms[randoms[cubeX] + cubeY + 1] + cubeZ + 1],
				baa = randoms[randoms[randoms[cubeX + 1] + cubeY] + cubeZ],
				bba = randoms[randoms[randoms[cubeX + 1] + cubeY + 1] + cubeZ],
				bab = randoms[randoms[randoms[cubeX + 1] + cubeY] + cubeZ + 1],
				bbb = randoms[randoms[randoms[cubeX + 1] + cubeY + 1] + cubeZ + 1];

			//Calculate dot products of vectors from point to cube vertices with gradient vectors, and interpolate based on position within cube
			float x1 = cosineInterpolate(
				dotWithRandomDirection(aaa, lerptX, lerptY, lerptZ),
				dotWithRandomDirection(baa, lerptX - 1.f, lerptY, lerptZ),
				lerptX
			);

			float x2 = cosineInterpolate(
				dotWithRandomDirection(aba, lerptX, lerptY - 1.f, lerptZ),
				dotWithRandomDirection(bba, lerptX - 1.f, lerptY - 1.f, lerptZ),
				lerptX
			);

			float x3 = cosineInterpolate(
				dotWithRandomDirection(aab, lerptX, lerptY, lerptZ - 1.f),
				dotWithRandomDirection(bab, lerptX - 1.f, lerptY, lerptZ - 1.f),
				lerptX
			);

			float x4 = cosineInterpolate(
				dotWithRandomDirection(abb, lerptX, lerptY - 1.f, lerptZ - 1.f),
				dotWithRandomDirection(bbb, lerptX - 1.f, lerptY - 1.f, lerptZ - 1.f),
				lerptX
			);

			float y1 = cosineInterpolate(x1, x2, lerptY);
			float y2 = cosineInterpolate(x3, x4, lerptY);

			float z = cosineInterpolate(y1, y2, lerptZ);

			return (z + 1.f) / 2.f; // [-1, 1] -> [0, 1]
		}

		float PerlinNoise::cosineInterpolate(float _a, float _b, float _t) const
		{
			float f = (1 - cos(_t * 3.141592f)) * 0.5f;
			return _a * (1 - f) + _b * f;
		}

		float PerlinNoise::dotWithRandomDirection(int hash, float _x, float _y, float _z) const
		{
			//Choose from 
			// (1, 1, 0), (-1, 1, 0), (1, -1, 0), (-1, -1, 0),
			// (1, 0, 1), (-1, 0, 1), (1, 0, -1), (-1, 0, -1),
			// (0, 1, 1), (0, -1, 1), (0, 1, -1), (0, -1, -1)


			//Just verbose choose from options, and dot product with it
			switch (hash & 0xF)
			{
			case 0x0: return _x + _y; //{1, 1, 0}
			case 0x1: return -_x + _y; //{-1, 1, 0}
			case 0x2: return _x - _y; //{1, -1, 0}
			case 0x3: return -_x - _y; //{-1, -1, 0}

			case 0x4: return _x + _z; //{1, 0, 1}
			case 0x5: return -_x + _z; //{-1, 0, 1}
			case 0x6: return _x - _z; //{1, 0, -1}
			case 0x7: return -_x - _z; //{-1, 0, -1}

			case 0x8: return _y + _z; //{0, 1, 1}
			case 0x9: return -_y + _z; //{0, -1, 1}
			case 0xA: return _y - _z; //{0, 1, -1}
			case 0xB: return -_y - _z; //{0, -1, -1}

			//Not actually uniform since we must pick duplicates for C-F

			case 0xC: return _x + _y; //{1, 1, 0}
			case 0xD: return -_y + _z; //{0, -1, 1}
			case 0xE: return -_x + _y; //{-1, 1, 0}
			case 0xF: return -_y - _z; //{0, -1, -1}
			}

			return 0.0f; //Should never happen
		}
	}
}