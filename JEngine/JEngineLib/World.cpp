#include "World.h"

#include "Engine.h"
#include "Logger.h"
#include "Util.h"
#include "Entity.h"
#include "System.h"
#include "JobManagement.h"
#include "Renderer.h"
#include "ComponentRenderable.h"

#ifdef _DEBUG
#include "DebugRendering.h"
#endif

namespace JEngine
{

	void World::addToRenderMatrix(ComponentRenderable * _renderable, const int _entity)
	{
		ECS::Entity * entity = &entityManager.getEntity(_entity);

		auto & mapOfRenderersForThisShader = renderMatrix[_renderable->shader];

		mapOfRenderersForThisShader.insert(std::make_pair(_renderable->renderer, entity));
	}
	void World::removeFromRenderMatrix(ComponentRenderable * _renderable, const int _entity)
	{
		ECS::Entity * entity = &entityManager.getEntity(_entity);

		auto & mapOfRenderersForThisShader = renderMatrix[_renderable->shader];

		{ //Remove renderer/entity pair from the map
			auto entitiesWithThisRendererItrRange = mapOfRenderersForThisShader.equal_range(_renderable->renderer);
			auto itr = entitiesWithThisRendererItrRange.first;

			for (; itr != entitiesWithThisRendererItrRange.second; ++itr)
			{
				if (itr->second == entity)
				{
					mapOfRenderersForThisShader.erase(itr);
					break;
				}
			}
		}

		//Remove map for this shader if no renderer/entity pairs present
		if (mapOfRenderersForThisShader.size() == 0)
		{
			renderMatrix.erase(_renderable->shader);
		}
	}

	void World::drawRenderMatrix() const
	{
		Logger::get().log("-------------------");
		Logger::get().log("Render Matrix Debug");
		Logger::get().log("-------------------");
		
		for (auto itr = renderMatrix.begin(); itr != renderMatrix.end(); ++itr)
		{
			Logger::get().log(strJoinConvert("Shader: ", itr->first));

			void * lastRenderer = nullptr;

			for (auto itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2)
			{
				if (itr2->first != lastRenderer)
				{
					Logger::get().log(strJoinConvert("   |-- Renderer: ", itr2->first));
				}

				Logger::get().log(strJoinConvert("      |-- Entity: ", itr2->second));
			}
		}
		Logger::get().log("-------------------");
	}

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
		ERR_IF(!entityManager.initialise(), "Could not initialise entity manager", "Initialised entity manager");
		ERR_IF(!componentManager.initialise(), "Could not initialise component manager", "Initialised component manager");
		ERR_IF(!systemManager.initialise(), "Could not initialise system manager", "Initialised system manager");

		return true;
	}

	void World::update()
	{
	}

	void World::render() const
	{
		drawRenderMatrix();



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