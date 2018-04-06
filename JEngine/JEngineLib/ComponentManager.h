#pragma once

#include <map>

#include "ECSConstants.h"
#include "PoolAllocator.h"
#include "Component.h"

namespace JEngine
{
	namespace ECS
	{

		class ComponentManager
		{
		private:
			RawPoolAllocator<MAX_SIZE_PER_COMPONENT, MAX_COMPONENTS> componentAllocator;
			std::map<size_t, std::map<int, void *>> components;


		public:
			ComponentManager();
			~ComponentManager();

			bool initialise();

			void clearComponents(int _entity);

			template <typename T>
			T & getComponent(int _entity);

			template <typename T, typename ... Args>
			T & createComponent(int _entity, Args ... _args);

			template <typename T>
			void destroyComponent(int _entity);

		};

		template<typename T>
		inline T & ComponentManager::getComponent(int _entity)
		{
			//Find list of components (by entity id) of this type
			auto & currComponent = components[typeid(T).hash_code()];

			//Return the component for this entity
			return *reinterpret_cast<T*>(currComponent[_entity]);
		}

		template <typename T, typename ... Args>
		inline T & ComponentManager::createComponent(int _entity, Args ... _args)
		{
			//Find list of components (by entity id) of this type
			auto & currComponent = components[typeid(T).hash_code()];

			//Allocate new component
			T * allocated = componentAllocator.allocateRaw<T>(_args...);

			//Set component in map
			currComponent[_entity] = allocated;
		}

		template<typename T>
		inline void ComponentManager::destroyComponent(int _entity)
		{
			//Find list of components (by entity id) of this type
			auto & currComponent = components[typeid(T).hash_code()];

			//Find component memory
			T * allocated = currComponent[_entity];

			//Clear address in map
			currComponent[_entity] = nullptr;

			//Return allocated memory to pool
			componentAllocator.deallocate(allocated);
		}
	}
}