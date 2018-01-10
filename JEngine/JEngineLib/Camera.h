#pragma once

#include "Maths.h"
#include "Transform.h"

namespace JEngine
{
	enum ProjectionType
	{
		PERSPECTIVE,
		ORTHOGRAPHIC
	};

	class Camera : public Transform
	{
	private:
		float zNear, zFar;
		float fov;

		ProjectionType projectionType;

		mat4 viewMatrix; //Inverse of transformMatrix in 'Transform'
		mat4 projectionMatrix; //Perspective/Ortho
		mat4 viewProjectionMatrix; //VP Optimisation matrix

		void cameraFlush();

	public:
		Camera(ProjectionType _projectionType, float _zNear, float _zFar);
		Camera(ProjectionType _projectionType, float _fov, float _zNear, float _zFar);
		~Camera();

		void flush();

		const mat4 & getViewMatrix() const;
		const mat4 & getProjectionMatrix() const;
		const mat4 & getViewProjectionMatrix() const;

	};
}