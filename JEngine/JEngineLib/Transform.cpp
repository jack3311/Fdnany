#include "Transform.h"

namespace JEngine
{
	Transform::Transform() :
		position(),
		rotation(),
		scale(1.f, 1.f, 1.f),
		localTransformMatrix()
	{
		flush();
	}

	Transform::~Transform()
	{
	}

	void Transform::flush()
	{
		//Update local transform matrix
		localTransformMatrix = MathUtil::gen(position, rotation, scale);
	}

	const vec3 & Transform::getLocalPosition() const
	{
		return position;
	}
	const vec3 & Transform::getLocalScale() const
	{
		return scale;
	}
	const quat & Transform::getLocalRotation() const
	{
		return rotation;
	}
	const mat4 & Transform::getLocalTransformMatrix() const
	{
		return localTransformMatrix;
	}
	Transform & Transform::localMove(const vec3 & _delta)
	{
		position += _delta;
		return *this;
	}
	Transform & Transform::localScale(const vec3 & _multiplier)
	{
		scale *= _multiplier;
		return *this;
	}
	Transform & Transform::localRotate(const quat & _other)
	{
		rotation *= _other;
		return *this;
	}
	Transform & Transform::localSetPosition(const vec3 & _value)
	{
		position = _value;
		return *this;
	}
	Transform & Transform::localSetScale(const vec3 & _value)
	{
		scale = _value;
		return *this;
	}
	Transform & Transform::localSetRotation(const quat & _value)
	{
		rotation = _value;
		return *this;
	}
	Transform & Transform::localLookAt(const vec3 & _lookDir, const vec3 & _up)
	{
		rotation = MathUtil::lookAtQuat(_lookDir, _up);
		return *this;
	}
}