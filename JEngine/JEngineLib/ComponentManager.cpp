#include "ComponentManager.h"

#include "Util.h"
#include "Logger.h"


ComponentManager::ComponentManager()
{
	
}


ComponentManager::~ComponentManager()
{
}

bool JEngine::ECS::ComponentManager::initialise()
{
	ERR_IF(!componentAllocator.initialise(), "Could not initialise component allocator");
	
	return true;
}

void JEngine::ECS::ComponentManager::clearComponents(int _entity)
{
}
