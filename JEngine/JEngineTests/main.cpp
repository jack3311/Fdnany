#include <JEngineLib\Engine.h>
#include <JEngineLib\Maths.h>
#include <JEngineLib\SceneManagement.h>
#include <JEngineLib\JobManagement.h>

#include "TestScene.h"
#include "TestJob.h"


int main()
{
	JEngine::Engine::startup();
	JEngine::Engine & engine = JEngine::Engine::getEngine();

	engine.initialise("Test Game", "log.txt");

	auto sceneID = engine.getSceneManager().registerScene(std::make_shared<TestScene>());
	engine.getSceneManager().pushScene(sceneID);

	std::vector<std::shared_ptr<TestJob>> jobs(5000);

	//Add some test jobs
	for (int i = 0; i < 5000; ++i)
	{
		jobs[i] = std::make_shared<TestJob>(i);
		engine.getJobManager().enqueueJob(jobs[i]);
		//job->execute();
	}

	
	engine.start();

	engine.cleanUp();

	return 0;
}