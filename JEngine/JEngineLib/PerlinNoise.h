#include <random>

namespace JEngine
{
	namespace Maths
	{
		class PerlinNoise {
		public:
			PerlinNoise(unsigned int seed);

			float octaveNoise(float _x, float _y, float _z) const;

			float noise(float _x, float _y, float _z) const;

		private:
			std::vector<int> randoms;

			std::mt19937 gen;

			unsigned int width, height;


			float cosineInterpolate(float _a, float _b, float _t) const;

			float dotWithRandomDirection(int hash, float _x, float _y, float _z) const;
		};
	}
}