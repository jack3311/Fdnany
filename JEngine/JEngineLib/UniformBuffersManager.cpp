#include "UniformBuffersManager.h"

#include "Util.h"
#include "Logger.h"


namespace JEngine
{
	UniformBuffersManager::UniformBuffersManager()
	{
	}

	UniformBuffersManager::~UniformBuffersManager()
	{
	}

	bool UniformBuffersManager::initialise()
	{
		ERR_IF(!uniformBufferViewInfo->initialise(UNIFORM_BUFFER_VIEW_INFO_BINDING_LOCATION), "Failed to initialise view uniform buffer", "Initialised view uniform buffer");

		return true;
	}
}
