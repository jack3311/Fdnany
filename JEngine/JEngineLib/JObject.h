#pragma once

#include <vector>
#include <memory>

#include "Maths.h"
#include "JobManagement.h"

namespace JEngine
{
	class JObject
	{
	private:
		static JObject * defaultTransform;
	public:
		static const JObject & getDefaultTransform();

	protected:
		vec3 position;
		vec3 scale;
		quat rotation;

		mat4 localTransformMatrix;
		mat4 globalTransformMatrix;

		JObject * parent;
		std::vector<std::shared_ptr<JObject>> children;

	//public: //TODO: make protected as above
		void updateGlobalTransformMatrixRecursive();
		void updateGlobalTransformMatrixRecursiveAsync(std::shared_ptr<JobAggregate> _jobAggregate);

	public:
		JObject();
		~JObject();

		void flush();

		const vec3 & getLocalPosition() const;
		const vec3 & getLocalScale() const;
		const quat & getLocalRotation() const;

		vec3 getGlobalPosition() const;

		const mat4 & getLocalTransformMatrix() const;
		const mat4 & getGlobalTransformMatrix() const;


		JObject & localMove(const vec3 & _delta);
		JObject & localScale(const vec3 & _multiplier);
		JObject & localRotate(const quat & _other);

		JObject & localSetPosition(const vec3 & _value);
		JObject & localSetScale(const vec3 & _value);
		JObject & localSetRotation(const quat & _value);

		JObject & localLookAt(const vec3 & _lookDir, const vec3 & _up = { 0.f, 1.f, 0.f });

		void addChild(const std::shared_ptr<JObject> & _child);
		void removeChild(const std::shared_ptr<JObject> & _child);

		const std::vector<std::shared_ptr<JObject>> & getChildren() const; //TODO: maybe remove


#ifdef _DEBUG
		void drawDebugInfo() const;
#endif
	};
}
