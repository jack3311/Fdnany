#pragma once

#include "Maths.h"
#include "Transform.h"

namespace JEngine
{
#define DEFAULT_FOV 90.f
#define DEFAULT_Z_NEAR 0.1f
#define DEFAULT_Z_FAR 100.f

	enum ProjectionType
	{
		PERSPECTIVE,
		ORTHOGRAPHIC
	};

	template <ProjectionType projectionType>
	class Camera : public Transform
	{
	private:
		fvec3 position;
		float zNear, zFar;
		std::enable_if_t<projectionType == ProjectionType::PERSPECTIVE, float> fov;

		fmat4x4 projectionMatrix; //Perspective/Ortho

		void cameraFlush();

	public:
		Camera(float _zNear = DEFAULT_Z_NEAR, float _zFar = DEFAULT_Z_FAR);
		Camera(std::enable_if<projectionType == ProjectionType::PERSPECTIVE, float> _fov = DEFAULT_FOV, float _zNear = DEFAULT_Z_NEAR, float _zFar = DEFAULT_Z_FAR);
		~Camera();

		void flush();
	};

	template<ProjectionType projectionType>
	inline void Camera<projectionType>::cameraFlush()
	{
		auto & engine = Engine::getEngine();
		auto & windowSize = engine.getWindowSizeFloat();

		if (projectionType == ProjectionType::PERSPECTIVE)
		{
			projectionMatrix = transform::ortho_mat<float>(windowSize.x, windowSize.y, zNear, zFar);
		}
		else if (projectionType == ProjectionType::ORTHOGRAPHIC)
		{
			projectionMatrix = transform::perspective_mat()

		}
	}

	template<ProjectionType projectionType>
	inline Camera<projectionType>::Camera(float _zNear, float _zFar) :
		Transform(),
		zNear(_zNear),
		zFar(_zFar)
	{
		cameraFlush();
	}

	template<ProjectionType projectionType>
	inline Camera<projectionType>::Camera(std::enable_if<projectionType == ProjectionType::PERSPECTIVE, float> _fov, float _zNear, float _zFar) :
		Transform(),
		fov(_fov),
		zNear(_zNear),
		zFar(_zFar)
	{
		cameraFlush();
	}

	template<ProjectionType projectionType>
	inline Camera<projectionType>::~Camera()
	{
	}

	template<ProjectionType projectionType>
	inline void Camera<projectionType>::flush()
	{
		Transform::flush();
		cameraFlush();
	}
}