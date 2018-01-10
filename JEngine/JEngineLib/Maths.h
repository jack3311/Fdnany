#pragma once

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\matrix_inverse.hpp>
#include <gtx\euler_angles.hpp>
#include <gtc\type_ptr.hpp>

using namespace glm;

namespace JEngine
{
	class MatrixUtil
	{
	public:
		static mat4 gen(const vec3 & _position, const vec3 & _rotation, const vec3 & _scale)
		{
			mat4 result;

			result = translate(result, _position);

			mat4 eulerRotationMatrix = eulerAngleXYZ(_rotation.x, _rotation.y, _rotation.z);
			result *= eulerRotationMatrix;

			scale(result, _scale);

			return result;
		}
	};
}