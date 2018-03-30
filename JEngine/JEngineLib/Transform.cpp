#include "Transform.h"

#include "Logger.h"
#include "MathUtil.h"

namespace JEngine
{
	void Transform::updateGlobalTransformMatrixRecursive()
	{
		globalTransformMatrix = (parent ? parent->getGlobalTransformMatrix() : mat4()) * localTransformMatrix;
		
		for (const auto & child : children)
		{
			child->updateGlobalTransformMatrixRecursive();
		}
	}

	void Transform::updateGlobalTransformMatrixRecursiveAsync(std::shared_ptr<JobAggregate> _jobAggregate)
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

	Transform::Transform() :
		position(),
		rotation(),
		scale(1.f, 1.f, 1.f),
		localTransformMatrix(),
		globalTransformMatrix(),
		parent(nullptr)
	{
		flush();
	}

	Transform::~Transform()
	{

	}

	void Transform::flush()
	{
		//Update local transform matrix
		localTransformMatrix = Maths::MathUtil::mat4FromPosRotScl(position, rotation, scale);
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
	vec3 Transform::getGlobalPosition() const
	{
		return vec3(globalTransformMatrix * vec4{ 0.f, 0.f, 0.f, 1.f });
	}
	const mat4 & Transform::getLocalTransformMatrix() const
	{
		return localTransformMatrix;
	}
	const mat4 & Transform::getGlobalTransformMatrix() const
	{
		return globalTransformMatrix;
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
		rotation = Maths::MathUtil::lookAtQuat(_lookDir, _up);
		return *this;
	}
	void Transform::addChild(const std::shared_ptr<Transform> & _child)
	{
		assert(!_child->parent);
		_child->parent = this;
		children.push_back(_child);
	}
	void Transform::removeChild(const std::shared_ptr<Transform> & _child)
	{
		auto itr = std::find(children.begin(), children.end(), _child);

		if (itr != children.end())
		{
			assert((*itr)->parent == this);
			(*itr)->parent = nullptr;
			children.erase(itr);
		}
		else
		{
			Logger::getLogger().log("Cannot remove transform that is not a child", LogLevel::WARNING);
		}
	}
	const std::vector<std::shared_ptr<Transform>> & Transform::getChildren() const
	{
		return children;
	}
}