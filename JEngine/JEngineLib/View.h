#pragma once

#include <memory>

namespace JEngine
{
	class Camera;

	class View
	{
	private:
		std::shared_ptr<Camera> camera;

	public:
		View(const std::shared_ptr<Camera> & _camera);
		~View();

		bool initialise();

		const std::shared_ptr<Camera> & getCamera() const;
	};
}