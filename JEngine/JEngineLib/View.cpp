#include "View.h"

#include "Camera.h"

namespace JEngine
{
	View::View(const std::shared_ptr<Camera> & _camera) :
		camera(_camera)
	{
	}

	View::~View()
	{
	}

	bool View::initialise()
	{
		camera->flush();

		return true;
	}

	const std::shared_ptr<Camera> & View::getCamera() const
	{
		return camera;
	}
}