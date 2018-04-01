#pragma once

#include <map>

#include "PoolAllocator.h"
#include "Entity.h"

namespace JEngine
{
	namespace ECS
	{
#define MAX_ENTITIES 10000
#define MAX_SIZE_PER_ENTITY sizeof(int) * 100

		class EntityManager
		{
		private:
			int currentEntityID;

			RawPoolAllocator<MAX_SIZE_PER_ENTITY, MAX_ENTITIES> entityAllocator;
			std::map<int, Entity *> entities;
			Entity * root;
			

		public:
			EntityManager(int _startingEntityID = 0);
			~EntityManager();

			bool initialise();

			Entity & createEntity();
			void destroyEntity(Entity & _entity);

			Entity & getEntity(int _entity);
			Entity & getRoot();
			const Entity & getRoot() const;
		};
	}
}