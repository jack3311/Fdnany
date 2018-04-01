#pragma once

#include <vector>
#include <memory>

#include "Maths.h"
#include "JobManagement.h"

namespace JEngine
{
	namespace ECS
	{
		class Entity
		{
		private:
			int objectID;

		protected:
			vec3 position;
			vec3 scale;
			quat rotation;

			mat4 localTransformMatrix;
			mat4 globalTransformMatrix;

			Entity * parent;
			std::vector<Entity *> children;

			void addChild(Entity * _child);
			void removeChild(Entity * _child);

		public: //TODO: make protected as above
			void updateGlobalTransformMatrixRecursive();
			void updateGlobalTransformMatrixRecursiveAsync(std::shared_ptr<JobAggregate> _jobAggregate);

		public:
			Entity(int _objectID);
			Entity(const Entity & _other) = delete;
			Entity(const Entity && _other) = delete;
			~Entity();

			const Entity & operator=(const Entity & _other) = delete;
			const Entity & operator=(const Entity && _other) = delete;

			int getEntityID() const;

			void flush();

			const vec3 & getLocalPosition() const;
			const vec3 & getLocalScale() const;
			const quat & getLocalRotation() const;

			vec3 getGlobalPosition() const;

			const mat4 & getLocalTransformMatrix() const;
			const mat4 & getGlobalTransformMatrix() const;


			Entity & localMove(const vec3 & _delta);
			Entity & localScale(const vec3 & _multiplier);
			Entity & localRotate(const quat & _other);

			Entity & localSetPosition(const vec3 & _value);
			Entity & localSetScale(const vec3 & _value);
			Entity & localSetRotation(const quat & _value);

			Entity & localLookAt(const vec3 & _lookDir, const vec3 & _up = { 0.f, 1.f, 0.f });
			


			const std::vector<Entity *> & getChildren() const;

			Entity * getParent() const;
			void setParent(Entity * _parent);


#ifdef _DEBUG
			void drawDebugInfo() const;
#endif

			//void operator delete(void *) = delete;
			//void operator delete[](void *) = delete;
		};
	}
}
