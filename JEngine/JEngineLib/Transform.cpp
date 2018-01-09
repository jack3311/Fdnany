#include "Transform.h"

namespace JEngine
{
	Transform::Transform() :
		position(fvec3::zero()),
		eulerAngles(fvec3::zero()),
		scale(fvec3::zero()),
		localTransformMatrix(fmat4x4::zero())
	{
		flush();
	}

	Transform::~Transform()
	{
	}

	void Transform::flush()
	{
		//Update local transform matrix
		localTransformMatrix = MatrixUtil::gen(position, eulerAngles, scale);
	}

	const fvec3 & Transform::getLocalPosition() const
	{
		return position;
	}
	const fvec3 & Transform::getLocalEulerAngles() const
	{
		return eulerAngles;
	}
	const fvec3 & Transform::getLocalScale() const
	{
		return scale;
	}
	const fmat4x4 & Transform::getLocalTransformMatrix() const
	{
		return localTransformMatrix;
	}
	Transform & Transform::localMove(const fvec3 & _delta)
	{
		position += _delta;
		return *this;
	}
	Transform & Transform::localRotateEulerAngles(const fvec3 & _delta)
	{
		eulerAngles += _delta;
		return *this;
	}
	Transform & Transform::localScale(const fvec3 & _multiplier)
	{
		scale *= _multiplier;
		return *this;
	}
	Transform & Transform::localSetPosition(const fvec3 & _value)
	{
		position = _value;
		return *this;
	}
	Transform & Transform::localSetEulerAngles(const fvec3 & _value)
	{
		eulerAngles = _value;
		return *this;
	}
	Transform & Transform::localSetScale(const fvec3 & _value)
	{
		scale = _value;
		return *this;
	}
}