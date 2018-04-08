#include "SystemManager.h"

#include <typeinfo>
#include <algorithm>

#include "System.h"
#include "Entity.h"

namespace JEngine
{
	namespace ECS
	{

		SystemManager::SystemManager()
		{
		}


		SystemManager::~SystemManager()
		{
		}

		bool SystemManager::initialise()
		{
			return true;
		}

		void SystemManager::cleanUp()
		{
			systems.clear();
			systemOrder.clear();
		}

		void SystemManager::preRender(Entity & _entity)
		{
			for (auto itr = systemOrder.begin(); itr != systemOrder.end(); ++itr)
			{
				itr->second->preRender(_entity);
			}
		}

		void SystemManager::postRender(Entity & _entity)
		{
			for (auto itr = systemOrder.begin(); itr != systemOrder.end(); ++itr)
			{
				itr->second->postRender(_entity);
			}
		}
	}
}
