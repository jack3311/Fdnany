#include "Camera.h"

#include "Engine.h"

namespace JEngine
{
	void Camera::cameraFlush()
	{
		auto & engine = Engine::get();
		auto & windowSize = engine.getWindowSizeFloat();

		//Update projection matrix
		if (projectionType == ProjectionType::PERSPECTIVE)
		{
			float aspect = windowSize.x / windowSize.y;
			projectionMatrix = perspective(fov, aspect, zNear, zFar);
		}
		else if (projectionType == ProjectionType::ORTHOGRAPHIC)
		{
			projectionMatrix = ortho(0.f, windowSize.x, 0.f, windowSize.y/*, zNear, zFar*/);
		}

		//Update view matrix
		viewMatrix = inverse(localTransformMatrix); //TODO: NOT LOCAL TRANSFORM MATRIX, BUT GLOBAL (in case camera is child)

		//Update VP matrix
		viewProjectionMatrix = projectionMatrix * viewMatrix;
	}

	Camera::Camera(ProjectionType _projectionType, float _zNear, float _zFar) :
		Entity(1000), //TODO: FIX ME
		projectionType(_projectionType),
		zNear(_zNear),
		zFar(_zFar)
	{
	}

	Camera::Camera(ProjectionType _projectionType, float _fov, float _zNear, float _zFar) :
		Entity(1000), //TODO: FIX ME
		fov(_fov),
		projectionType(_projectionType),
		zNear(_zNear),
		zFar(_zFar)
	{
	}

	Camera::~Camera()
	{
	}

	void Camera::flush()
	{
		Entity::flush();
		cameraFlush();
	}
	const mat4 & Camera::getViewMatrix() const
	{
		return viewMatrix;
	}
	const mat4 & Camera::getProjectionMatrix() const
	{
		return projectionMatrix;
	}
	const mat4 & Camera::getViewProjectionMatrix() const
	{
		return viewProjectionMatrix;
	}
}