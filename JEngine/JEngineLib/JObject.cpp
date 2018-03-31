#include "JObject.h"

#include "Logger.h"
#include "MathUtil.h"

#ifdef _DEBUG
#include "DebugRendering.h"
#endif

namespace JEngine
{
	JObject * JObject::defaultTransform = nullptr;

	const JObject & JObject::getDefaultTransform()
	{
		if (!defaultTransform)
		{
			defaultTransform = new JObject();
			defaultTransform->localSetPosition({ 0, 0, 0 });
			defaultTransform->localSetRotation(quat());
			defaultTransform->localSetScale({ 1, 1, 1 });
		}
		return *defaultTransform;
	}



	void JObject::updateGlobalTransformMatrixRecursive()
	{
		globalTransformMatrix = (parent ? parent->getGlobalTransformMatrix() : mat4()) * localTransformMatrix;
		
		for (const auto & child : children)
		{
			child->updateGlobalTransformMatrixRecursive();
		}
	}

	void JObject::updateGlobalTransformMatrixRecursiveAsync(std::shared_ptr<JobAggregate> _jobAggregate)
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

	JObject::JObject() :
		position(),
		rotation(),
		scale(1.f, 1.f, 1.f),
		localTransformMatrix(),
		globalTransformMatrix(),
		parent(nullptr)
	{
		flush();
	}

	JObject::~JObject()
	{

	}

	void JObject::flush()
	{
		//Update local transform matrix
		localTransformMatrix = Maths::MathUtil::mat4FromPosRotScl(position, rotation, scale);
	}

	const vec3 & JObject::getLocalPosition() const
	{
		return position;
	}
	const vec3 & JObject::getLocalScale() const
	{
		return scale;
	}
	const quat & JObject::getLocalRotation() const
	{
		return rotation;
	}
	vec3 JObject::getGlobalPosition() const
	{
		return vec3(globalTransformMatrix * vec4{ 0.f, 0.f, 0.f, 1.f });
	}
	const mat4 & JObject::getLocalTransformMatrix() const
	{
		return localTransformMatrix;
	}
	const mat4 & JObject::getGlobalTransformMatrix() const
	{
		return globalTransformMatrix;
	}
	JObject & JObject::localMove(const vec3 & _delta)
	{
		position += _delta;
		return *this;
	}
	JObject & JObject::localScale(const vec3 & _multiplier)
	{
		scale *= _multiplier;
		return *this;
	}
	JObject & JObject::localRotate(const quat & _other)
	{
		rotation *= _other;
		return *this;
	}
	JObject & JObject::localSetPosition(const vec3 & _value)
	{
		position = _value;
		return *this;
	}
	JObject & JObject::localSetScale(const vec3 & _value)
	{
		scale = _value;
		return *this;
	}
	JObject & JObject::localSetRotation(const quat & _value)
	{
		rotation = _value;
		return *this;
	}
	JObject & JObject::localLookAt(const vec3 & _lookDir, const vec3 & _up)
	{
		rotation = Maths::MathUtil::lookAtQuat(_lookDir, _up);
		return *this;
	}
	void JObject::addChild(const std::shared_ptr<JObject> & _child)
	{
		assert(!_child->parent);
		_child->parent = this;
		children.push_back(_child);
	}
	void JObject::removeChild(const std::shared_ptr<JObject> & _child)
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
			Logger::get().log("Cannot remove JObject that is not a child", LogLevel::WARNING);
		}
	}
	const std::vector<std::shared_ptr<JObject>> & JObject::getChildren() const
	{
		return children;
	}


#ifdef _DEBUG
	void JObject::drawDebugInfo() const
	{
		DebugRendering::get().drawTransform(*this);

		for (const auto & child : getChildren())
		{
			child->drawDebugInfo();
		}
	}
#endif
}