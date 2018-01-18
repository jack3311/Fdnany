#pragma once

#include "Maths.h"

namespace JEngine
{
	class Transform
	{
	protected:
		vec3 position;
		vec3 scale;
		quat rotation;

		mat4 localTransformMatrix;

		//std::vector<Transform &> children;

	public:
		Transform();
		~Transform();

		void flush();

		const vec3 & getLocalPosition() const;
		const vec3 & getLocalScale() const;
		const quat & getLocalRotation() const;

		const mat4 & getLocalTransformMatrix() const;


		Transform & localMove(const vec3 & _delta);
		Transform & localScale(const vec3 & _multiplier);
		Transform & localRotate(const quat & _other);

		Transform & localSetPosition(const vec3 & _value);
		Transform & localSetScale(const vec3 & _value);
		Transform & localSetRotation(const quat & _value);

		Transform & localLookAt(const vec3 & _lookDir, const vec3 & _up = { 0.f, 1.f, 0.f });
	};
}
