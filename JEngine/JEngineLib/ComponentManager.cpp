#include "ComponentManager.h"

#include "Util.h"
#include "Logger.h"


namespace JEngine
{
	namespace ECS
	{
		ComponentManager::ComponentManager()
		{

		}


		ComponentManager::~ComponentManager()
		{
		}

		bool ComponentManager::initialise()
		{
			ERR_IF(!componentAllocator.initialise(), "Could not initialise component allocator");

			return true;
		}

		void ComponentManager::clearComponents(int _entity)
		{
		}
	}
}