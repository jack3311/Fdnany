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

		std::multimap<unsigned int, System *, std::less<unsigned int>> & SystemManager::getSystemsOrdered()
		{
			return systemOrder;
		}
	}
}
