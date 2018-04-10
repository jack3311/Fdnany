#include "ComponentManager.h"

#include "Util.h"
#include "Logger.h"


namespace JEngine
{
	namespace ECS
	{
		void ComponentManager::destroyComponentInternal(int _entity, std::map<int, void *> & _components)
		{
			//Find component memory
			void * allocated = _components[_entity];

			//Clear address in map
			_components[_entity] = nullptr;

			//Return allocated memory to pool
			componentAllocator.deallocate(allocated);
		}

		ComponentManager::ComponentManager()
		{

		}


		ComponentManager::~ComponentManager()
		{
		}

		bool ComponentManager::initialise()
		{
			assert(Engine::get().isCurrentThreadMain());

			ERR_IF(!componentAllocator.initialise(), "Could not initialise component allocator", "Initilised component allocator");

			return true;
		}

		void ComponentManager::cleanUp()
		{
			components.clear();
			componentAllocator.cleanUp();
		}

		void ComponentManager::clearComponents(const int _entity)
		{
			assert(Engine::get().isCurrentThreadMain());

			for (auto itr = components.begin(); itr != components.end(); ++itr)
			{
				auto & thisComponentTypeMap = itr->second;
				
				destroyComponentInternal(_entity, thisComponentTypeMap);
			}
		}
	}
}