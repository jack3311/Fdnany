#pragma once

#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

namespace JEngine
{
	class World
	{
	private:
		ECS::EntityManager entityManager;
		ECS::ComponentManager componentManager;
		ECS::SystemManager systemManager;

		void updateEntityMatrices();

	public:
		World();
		~World();

		bool initialise();

		void update();
		void render() const;

		ECS::EntityManager & getEntityManager();
		ECS::ComponentManager & getComponentManager();
		ECS::SystemManager & getSystemManager();

		void preRender();
		void postRender();
	};
}
