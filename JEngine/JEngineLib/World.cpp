#include "World.h"

#include "Engine.h"
#include "Logger.h"
#include "Util.h"

#ifdef _DEBUG
#include "DebugRendering.h"
#endif

namespace JEngine
{
	World::World() : JObject()
	{
	}


	World::~World()
	{
	}

	bool World::initialise()
	{


		return false;
	}

	void World::update()
	{
		//Update JObject and thus all of the scene objects
		std::shared_ptr<JEngine::JobAggregate> matrixUpdates = std::make_shared<JEngine::JobAggregate>();
		updateGlobalTransformMatrixRecursiveAsync(matrixUpdates);

		Logger::get().log(strJoinConvert("Updating ", matrixUpdates->getSubJobCount(), " JObjects"));

		Engine::get().getJobManager().enqueueJob(matrixUpdates);

		matrixUpdates->waitUntilAllSubJobsFinishedOrShutdown();
	}

	void World::render() const
	{
#ifdef _DEBUG
		//Draw axes
		DebugRendering::get().drawAxes();
		
		drawDebugInfo();
#endif
	}
}