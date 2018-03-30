#pragma once

#include "Maths.h"

namespace JEngine
{
	namespace Maths
	{
		class MathUtil
		{
		public:
			static mat4 mat4FromPosRotScl(const vec3 & _position, const quat & _rotation, const vec3 & _scale)
			{
				mat4 result;

				result = translate(result, _position);

				result *= toMat4(_rotation);

				result = scale(result, _scale);

				return result;
			}

			static quat lookAtQuat(const vec3 & _lookDir, const vec3 & _up = { 0.f, 1.f, 0.f })
			{
				return conjugate(toQuat(glm::lookAt({ 0.f, 0.f, 0.f }, _lookDir, _up)));
			}

			static mat4 lookAtMat(const vec3 & _lookDir, const vec3 & _up = { 0.f, 1.f, 0.f })
			{
				return inverse(glm::lookAt({ 0.f, 0.f, 0.f }, _lookDir, _up));
			}
		};
	}
}