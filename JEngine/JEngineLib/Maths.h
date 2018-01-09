#pragma once

#include <tue/mat.hpp>
#include <tue/math.hpp>
#include <tue/nocopy_cast.hpp>
#include <tue/quat.hpp>
#include <tue/simd.hpp>
#include <tue/sized_bool.hpp>
#include <tue/transform.hpp>
#include <tue/vec.hpp>

namespace JEngine
{
	using namespace tue;

	class MatrixUtil
	{
	public:
		static fmat4x4 gen(const fvec3 & _position, const fvec3 & _rotation, const fvec3 & _scale)
		{
			fmat4x4 translate = transform::translation_mat<float>(_position);
			fmat4x4 rotate = transform::rotation_mat<float>(_rotation);
			fmat4x4 scale = transform::scale_mat<float>(_scale);

			return translate * rotate * scale;
		}
	};
}