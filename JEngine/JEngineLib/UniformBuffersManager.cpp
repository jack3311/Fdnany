#include "UniformBuffersManager.h"

#include "Util.h"
#include "Logger.h"
#include "Engine.h"
#include "View.h"
#include "Camera.h"
#include "Constants.h"

namespace JEngine
{
	UniformBuffersManager::UniformBuffersManager()
	{
		uniformBufferViewInfo = std::make_unique<UniformBuffer<UniformBufferFormatViewInfo>>();
	}

	UniformBuffersManager::~UniformBuffersManager()
	{
	}

	bool UniformBuffersManager::initialise()
	{
		ERR_IF(!uniformBufferViewInfo->initialise(UNIFORM_BUFFER_VIEW_INFO_BINDING_LOCATION), "Failed to initialise view uniform buffer", "Initialised view uniform buffer");

		return true;
	}
	void UniformBuffersManager::updateUniformBuffers()
	{
		const Engine & engine = Engine::get();
		const View & currentView = engine.getCurrentView();
		const std::shared_ptr<Camera> & camera = currentView.getCamera();

		//View buffers
		{
			uniformBufferViewInfo->bufferData.view = camera->getViewMatrix();
			uniformBufferViewInfo->bufferData.projection = camera->getProjectionMatrix();
			uniformBufferViewInfo->bufferData.viewProjection = camera->getViewProjectionMatrix();
		}


		uniformBufferViewInfo->flushBufferUpdates();
	}
}
