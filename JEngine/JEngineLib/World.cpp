#include "World.h"

#include "Engine.h"
#include "Logger.h"
#include "Util.h"
#include "Entity.h"
#include "System.h"
#include "JobManagement.h"

#ifdef _DEBUG
#include "DebugRendering.h"
#endif

namespace JEngine
{
	void World::updateEntityMatrices()
	{
		//Update scene tree recursively
		std::shared_ptr<JEngine::JobAggregate> matrixUpdates = std::make_shared<JEngine::JobAggregate>();
		entityManager.getRoot().updateGlobalTransformMatrixRecursiveAsync(matrixUpdates);

		Logger::get().log(strJoinConvert("Updating ", matrixUpdates->getSubJobCount(), " JObjects"));

		Engine::get().getJobManager().enqueueJob(matrixUpdates);

		matrixUpdates->waitUntilAllSubJobsFinishedOrShutdown();
	}

	World::World()
	{
	}


	World::~World()
	{
	}

	bool World::initialise()
	{
		ERR_IF(!entityManager.initialise(), "Could not initialise entity manager");

		return true;
	}

	void World::update()
	{
	}

	void World::render() const
	{
#ifdef _DEBUG
		//Draw axes
		DebugRendering::get().drawAxes();
		
		entityManager.getRoot().drawDebugInfo();
#endif
	}


	ECS::EntityManager & World::getEntityManager()
	{
		return entityManager;
	}
	ECS::ComponentManager & World::getComponentManager()
	{
		return componentManager;
	}
	ECS::SystemManager & World::getSystemManager()
	{
		return systemManager;
	}


	void World::preRender()
	{
		auto & systems = systemManager.getSystemsOrdered();
		auto & entities = entityManager.getEntities();

		for (auto systemItr = systems.begin(); systemItr != systems.end(); ++systemItr)
		{
			for (auto entityItr = entities.begin(); entityItr != entities.end(); ++entityItr)
			{
				systemItr->second->preRender(*entityItr->second);
			}
		}


		updateEntityMatrices();
	}
	void World::postRender()
	{
		auto & systems = systemManager.getSystemsOrdered();
		auto & entities = entityManager.getEntities();

		for (auto systemItr = systems.begin(); systemItr != systems.end(); ++systemItr)
		{
			for (auto entityItr = entities.begin(); entityItr != entities.end(); ++entityItr)
			{
				systemItr->second->postRender(*entityItr->second);
			}
		}
	}
}