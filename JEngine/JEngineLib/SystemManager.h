#pragma once

#include <map>
#include <vector>

#include "Engine.h"
#include "StackAllocator.h"

namespace JEngine
{
	namespace ECS
	{
		class System;
		class Entity;

		class SystemManager
		{
		private:
			std::map<size_t, System *> systems;
			std::multimap<unsigned int, System *, std::less<unsigned int>> systemOrder;

		public:
			SystemManager();
			~SystemManager();

			bool initialise();

			void cleanUp();

			template <typename T, typename ... Args>
			void registerSystem(Args ... _args);

			std::multimap<unsigned int, System *, std::less<unsigned int>> & getSystemsOrdered();
		};

		template<typename T, typename ... Args>
		inline void SystemManager::registerSystem(Args ... _args)
		{
			assert(Engine::get().isCurrentThreadMain());
			assert(systems[typeid(T).hash_code()] == nullptr);



			//Allocate system
			T * allocated = new T(_args...);

			//Add to main system map
			systems[typeid(T).hash_code()] = allocated;
			
			//Add to ordered system map
			systemOrder.emplace(std::pair<unsigned int, System *>(T::getExecutionPriority(), allocated));
			//systemOrder[T::getExecutionPriority()] = allocated;
		}
	}
}

