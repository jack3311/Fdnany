#include "Transform.h"

namespace JEngine
{
	Transform::Transform() :
		position(),
		eulerAngles(),
		scale(),
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
		localTransformMatrix = MatrixUtil::gen(position, eulerAngles, scale);
	}

	const vec3 & Transform::getLocalPosition() const
	{
		return position;
	}
	const vec3 & Transform::getLocalEulerAngles() const
	{
		return eulerAngles;
	}
	const vec3 & Transform::getLocalScale() const
	{
		return scale;
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
	Transform & Transform::localRotateEulerAngles(const vec3 & _delta)
	{
		eulerAngles += _delta;
		return *this;
	}
	Transform & Transform::localScale(const vec3 & _multiplier)
	{
		scale *= _multiplier;
		return *this;
	}
	Transform & Transform::localSetPosition(const vec3 & _value)
	{
		position = _value;
		return *this;
	}
	Transform & Transform::localSetEulerAngles(const vec3 & _value)
	{
		eulerAngles = _value;
		return *this;
	}
	Transform & Transform::localSetScale(const vec3 & _value)
	{
		scale = _value;
		return *this;
	}
}