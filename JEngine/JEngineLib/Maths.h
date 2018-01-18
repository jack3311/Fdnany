#pragma once

#include <glm.hpp>
#include <gtx\quaternion.hpp>
#include <gtx\vector_angle.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\matrix_inverse.hpp>
#include <gtc\quaternion.hpp>
#include <gtx\euler_angles.hpp>
#include <gtc\type_ptr.hpp>

using namespace glm;

namespace JEngine
{
	class MathUtil
	{
	public:
		//static mat4 gen(const vec3 & _position, const vec3 & _rotation, const vec3 & _scale)
		static mat4 gen(const vec3 & _position, const quat & _rotation, const vec3 & _scale)
		{
			mat4 result;

			result = translate(result, _position);

			//mat4 eulerRotationMatrix = eulerAngleXYZ(_rotation.x, _rotation.y, _rotation.z);
			//result *= eulerRotationMatrix;

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