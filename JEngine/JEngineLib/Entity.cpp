#include "Entity.h"

#include "Logger.h"
#include "MathUtil.h"

#ifdef _DEBUG
#include "DebugRendering.h"
#endif

namespace JEngine
{
	namespace ECS
	{
		void Entity::updateGlobalTransformMatrixRecursive()
		{
			globalTransformMatrix = (parent ? parent->getGlobalTransformMatrix() : mat4()) * localTransformMatrix;

			for (const auto & child : children)
			{
				child->updateGlobalTransformMatrixRecursive();
			}
		}

		void Entity::updateGlobalTransformMatrixRecursiveAsync(std::shared_ptr<JobAggregate> _jobAggregate)
		{
			_jobAggregate->addJob(std::make_shared<JobCallFunction>([this]() -> bool {
				globalTransformMatrix = (parent ? parent->getGlobalTransformMatrix() : mat4()) * localTransformMatrix;
				return true;
			}));

			for (const auto & child : children)
			{
				child->updateGlobalTransformMatrixRecursiveAsync(_jobAggregate);
			}
		}

		Entity::Entity(int _objectID) :
			objectID(_objectID),
			position(),
			rotation(),
			scale(1.f, 1.f, 1.f),
			localTransformMatrix(),
			globalTransformMatrix(),
			parent(nullptr)
		{
			flush();
		}

		Entity::~Entity()
		{

		}

		int Entity::getEntityID() const
		{
			return objectID;
		}

		void Entity::flush()
		{
			//Update local transform matrix
			localTransformMatrix = Maths::MathUtil::mat4FromPosRotScl(position, rotation, scale);
		}

		const vec3 & Entity::getLocalPosition() const
		{
			return position;
		}
		const vec3 & Entity::getLocalScale() const
		{
			return scale;
		}
		const quat & Entity::getLocalRotation() const
		{
			return rotation;
		}
		vec3 Entity::getGlobalPosition() const
		{
			return vec3(globalTransformMatrix * vec4{ 0.f, 0.f, 0.f, 1.f });
		}
		const mat4 & Entity::getLocalTransformMatrix() const
		{
			return localTransformMatrix;
		}
		const mat4 & Entity::getGlobalTransformMatrix() const
		{
			return globalTransformMatrix;
		}
		Entity & Entity::localMove(const vec3 & _delta)
		{
			position += _delta;
			return *this;
		}
		Entity & Entity::localScale(const vec3 & _multiplier)
		{
			scale *= _multiplier;
			return *this;
		}
		Entity & Entity::localRotate(const quat & _other)
		{
			rotation *= _other;
			return *this;
		}
		Entity & Entity::localSetPosition(const vec3 & _value)
		{
			position = _value;
			return *this;
		}
		Entity & Entity::localSetScale(const vec3 & _value)
		{
			scale = _value;
			return *this;
		}
		Entity & Entity::localSetRotation(const quat & _value)
		{
			rotation = _value;
			return *this;
		}
		Entity & Entity::localLookAt(const vec3 & _lookDir, const vec3 & _up)
		{
			rotation = Maths::MathUtil::lookAtQuat(_lookDir, _up);
			return *this;
		}



		void Entity::addChild(Entity * _child)
		{
			assert(_child != nullptr);

			children.push_back(_child);
		}
		void Entity::removeChild(Entity * _child)
		{
			assert(_child != nullptr);

			auto itr = std::find(children.begin(), children.end(), _child);

			if (itr != children.end())
			{
				assert((*itr)->parent == this);
				//(*itr)->parent = nullptr;
				children.erase(itr);
			}
			else
			{
				Logger::get().log("Cannot remove Entity that is not a child", LogLevel::WARNING);
			}
		}


		const std::vector<Entity *> & Entity::getChildren() const
		{
			return children;
		}


		Entity * Entity::getParent() const
		{
			return parent;
		}

		void Entity::setParent(Entity * _parent)
		{
			if (parent != nullptr)
			{
				parent->removeChild(this);
			}

			parent = _parent;

			if (parent != nullptr)
			{
				parent->addChild(this);
			}
		}



#ifdef _DEBUG
		void Entity::drawDebugInfo() const
		{
			DebugRendering::get().drawTransform(*this);

			for (const auto & child : getChildren())
			{
				child->drawDebugInfo();
			}
		}
#endif
	}
}