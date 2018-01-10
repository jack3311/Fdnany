#include "Camera.h"

#include "Engine.h"

namespace JEngine
{
	void Camera::cameraFlush()
	{
		auto & engine = Engine::getEngine();
		auto & windowSize = engine.getWindowSizeFloat();

		//Update projection matrix
		if (projectionType == ProjectionType::PERSPECTIVE)
		{
			float aspect = windowSize.x / windowSize.y;
			projectionMatrix = perspective(fov, aspect, zNear, zFar);
		}
		else if (projectionType == ProjectionType::ORTHOGRAPHIC)
		{
			projectionMatrix = ortho(0.f, windowSize.x, 0.f, windowSize.y, zNear, zFar);
		}

		//Update view matrix
		viewMatrix = inverse(localTransformMatrix); //TODO: NOT LOCAL TRANSFORM MATRIX, BUT GLOBAL (in case camera is child)

		//Update VP matrix
		viewProjectionMatrix = projectionMatrix * viewMatrix;
	}
	Camera::Camera(ProjectionType _projectionType, float _zNear, float _zFar) :
		Transform(),
		projectionType(_projectionType),
		zNear(_zNear),
		zFar(_zFar)
	{
	}
	Camera::Camera(ProjectionType _projectionType, float _fov, float _zNear, float _zFar) :
		Transform(),
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
		Transform::flush();
		cameraFlush();
	}
}