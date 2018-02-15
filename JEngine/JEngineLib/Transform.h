#pragma once

#include <vector>
#include <memory>

#include "Maths.h"
#include "JobManagement.h"

namespace JEngine
{
	class Transform
	{
	protected:
		vec3 position;
		vec3 scale;
		quat rotation;

		mat4 localTransformMatrix;
		mat4 globalTransformMatrix;

		Transform * parent;
		std::vector<std::shared_ptr<Transform>> children;

	public: //TODO: make protected as above
		void updateGlobalTransformMatrixRecursive();
		void updateGlobalTransformMatrixRecursiveAsync(std::shared_ptr<JobAggregate> _jobAggregate);

	public:
		Transform();
		~Transform();

		void flush();

		const vec3 & getLocalPosition() const;
		const vec3 & getLocalScale() const;
		const quat & getLocalRotation() const;

		vec3 getGlobalPosition() const;

		const mat4 & getLocalTransformMatrix() const;
		const mat4 & getGlobalTransformMatrix() const;


		Transform & localMove(const vec3 & _delta);
		Transform & localScale(const vec3 & _multiplier);
		Transform & localRotate(const quat & _other);

		Transform & localSetPosition(const vec3 & _value);
		Transform & localSetScale(const vec3 & _value);
		Transform & localSetRotation(const quat & _value);

		Transform & localLookAt(const vec3 & _lookDir, const vec3 & _up = { 0.f, 1.f, 0.f });

		void addChild(const std::shared_ptr<Transform> & _child);
		void removeChild(const std::shared_ptr<Transform> & _child);

		const std::vector<std::shared_ptr<Transform>> & getChildren() const; //TODO: maybe remove
	};
}
