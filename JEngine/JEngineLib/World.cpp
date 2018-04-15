#include "World.h"

#include "Engine.h"
#include "Logger.h"
#include "Util.h"
#include "Entity.h"
#include "System.h"
#include "JobManagement.h"
#include "ComponentRenderable.h"
#include "Renderer.h"
#include "Material.h"

#ifdef _DEBUG
#include "DebugRendering.h"
#endif

namespace JEngine
{

	void World::addToRenderMatrix(ComponentRenderable * _renderable, const int _entity)
	{
		ECS::Entity * entity = &entityManager.getEntity(_entity);

		auto & mapOfRenderersForThisMaterial = renderMatrix[_renderable->material];

		mapOfRenderersForThisMaterial.insert(std::make_pair(_renderable->renderer, entity));
	}
	void World::removeFromRenderMatrix(ComponentRenderable * _renderable, const int _entity)
	{
		ECS::Entity * entity = &entityManager.getEntity(_entity);

		auto & mapOfRenderersForThisMaterial = renderMatrix[_renderable->material];

		{ //Remove renderer/entity pair from the map
			auto entitiesWithThisRendererItrRange = mapOfRenderersForThisMaterial.equal_range(_renderable->renderer);
			auto itr = entitiesWithThisRendererItrRange.first;

			for (; itr != entitiesWithThisRendererItrRange.second; ++itr)
			{
				if (itr->second == entity)
				{
					mapOfRenderersForThisMaterial.erase(itr);
					break;
				}
			}
		}

		//Remove map for this shader if no renderer/entity pairs present
		if (mapOfRenderersForThisMaterial.size() == 0)
		{
			renderMatrix.erase(_renderable->material);
		}
	}

	void World::drawRenderMatrix() const
	{
#ifdef _DEBUG
		Logger::get().log("-------------------");
		Logger::get().log("Render Matrix Debug");
		Logger::get().log("-------------------");
		
		for (auto itr = renderMatrix.begin(); itr != renderMatrix.end(); ++itr)
		{
			Logger::get().log(strJoinConvert("Material: ", itr->first));

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
#endif

		//Render the render matrix
		for (auto itr = renderMatrix.begin(); itr != renderMatrix.end(); ++itr)
		{
			//Begin material (uniform buffer, shader)
			itr->first->begin();


			RendererInterface * lastRenderer = nullptr;

			for (auto itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2)
			{
				if (itr2->first != lastRenderer)
				{
					//Swap renderers
					if (lastRenderer != nullptr)
					{
						lastRenderer->end();
					}

					itr2->first->begin();
				}

				//Load transform transformations
				itr->first->getShader().setTransformUniforms(*itr2->second);

				//Draw object
				itr2->first->draw();
			}
		}
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
		//Call pre-render on each system for each entity
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
		//Call post-render on each system for each entity
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