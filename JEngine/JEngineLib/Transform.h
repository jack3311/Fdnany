#pragma once

#include "Maths.h"

namespace JEngine
{
	class Transform
	{
	protected:
		vec3 position;
		vec3 eulerAngles;
		vec3 scale;

		mat4 localTransformMatrix;

		//std::vector<Transform &> children;

	public:
		Transform();
		~Transform();

		void flush();

		const vec3 & getLocalPosition() const;
		const vec3 & getLocalEulerAngles() const;
		const vec3 & getLocalScale() const;

		const mat4 & getLocalTransformMatrix() const;


		Transform & localMove(const vec3 & _delta);
		Transform & localRotateEulerAngles(const vec3 & _delta);
		Transform & localScale(const vec3 & _multiplier);

		Transform & localSetPosition(const vec3 & _value);
		Transform & localSetEulerAngles(const vec3 & _value);
		Transform & localSetScale(const vec3 & _value);
	};
}
