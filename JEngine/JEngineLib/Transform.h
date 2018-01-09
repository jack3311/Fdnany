#pragma once

#include "Maths.h"

namespace JEngine
{
	class Transform
	{
	private:
		fvec3 position;
		fvec3 eulerAngles;
		fvec3 scale;

		fmat4x4 localTransformMatrix;

		//std::vector<Transform &> children;

	public:
		Transform();
		~Transform();

		void flush();

		const fvec3 & getLocalPosition() const;
		const fvec3 & getLocalEulerAngles() const;
		const fvec3 & getLocalScale() const;

		const fmat4x4 & getLocalTransformMatrix() const;


		Transform & localMove(const fvec3 & _delta);
		Transform & localRotateEulerAngles(const fvec3 & _delta);
		Transform & localScale(const fvec3 & _multiplier);

		Transform & localSetPosition(const fvec3 & _value);
		Transform & localSetEulerAngles(const fvec3 & _value);
		Transform & localSetScale(const fvec3 & _value);
	};
}
