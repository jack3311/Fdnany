#include "ComponentManager.h"

#include "Util.h"
#include "Logger.h"


namespace JEngine
{
	namespace ECS
	{
		void ComponentManager::destroyComponentInternal(int _entity, size_t _componentTypeHash)
		{
			//Find list of components (by entity id) of this type
			auto & currComponent = components[typeid(T).hash_code()];

			//Find component memory
			T * allocated = reinterpret_cast<T*>(currComponent[_entity]);

			//Clear address in map
			currComponent[_entity] = nullptr;

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
			ERR_IF(!componentAllocator.initialise(), "Could not initialise component allocator");

			return true;
		}

		void ComponentManager::clearComponents(int _entity)
		{
			for (auto itr = components.begin(); itr != components.end(); ++itr)
			{
				auto thisComponentTypeMap = itr->second;
				

			}
		}
	}
}