#include "EntityManager.h"

#include "Util.h"
#include "Logger.h"

namespace JEngine
{
	namespace ECS
	{
		EntityManager::EntityManager(int _startingEntityID) :
			currentEntityID(_startingEntityID),
			root(nullptr)
		{
		}


		EntityManager::~EntityManager()
		{
			entityAllocator.cleanUp();
		}

		bool EntityManager::initialise()
		{
			ERR_IF(!entityAllocator.initialise(), "Could not initialise entity allocator", "Initialised entity allocator");
			
			//Create root entity
			Entity & rootRef = createEntity();
			root = &rootRef;

			return true;
		}


		Entity & EntityManager::createEntity()
		{
			++currentEntityID;

			//Allocate new entity from pool
			auto ptr = entityAllocator.allocateRaw<Entity>(currentEntityID);

			if (root)
				ptr->setParent(root);

			entities[currentEntityID] = ptr;

			return *ptr;
		}

		void EntityManager::destroyEntity(Entity & _entity)
		{
			int entityID = _entity.getEntityID();
			Entity * rawPointer = entities[entityID];

			//Clear parent's child
			_entity.setParent(nullptr);

			//Destroy children
			auto entityChildren = _entity.getChildren();
			for (Entity * child : entityChildren)
			{
				destroyEntity(*child);
			}

			//Clear from entity map
			entities[entityID] = nullptr;

			//Return memory to pool
			entityAllocator.deallocate(rawPointer);
		}

		Entity & EntityManager::getEntity(int _entity)
		{
			return *entities[_entity];
		}

		Entity & EntityManager::getRoot()
		{
			return *root;
		}

		const Entity & EntityManager::getRoot() const
		{
			return *root;
		}

		const std::map<int, Entity *> & EntityManager::getEntities() const
		{
			return entities;
		}


		
	}
}