#pragma once

#include "EntityManager.h"

namespace JEngine
{
	class World
	{
	private:
		ECS::EntityManager entityManager;

	public:
		World();
		~World();

		bool initialise();

		void update();
		void render() const;

		ECS::EntityManager & getEntityManager();
	};
}
