#include <JEngineLib\Engine.h>
#include <JEngineLib\Maths.h>
#include <JEngineLib\SceneManagement.h>
#include <JEngineLib\JobManagement.h>
#include <JEngineLib\Logger.h>
#include <JEngineLib\Util.h>
#include <JEngineLib\UI.h>

#include "TestScene.h"
#include "TestJob.h"


int main()
{
	JEngine::Engine::startup();
	JEngine::Engine & engine = JEngine::Engine::getEngine();

	engine.initialise("Test Game", "log.txt");

	auto sceneID = engine.getSceneManager().registerScene(std::make_shared<TestScene>());
	engine.getSceneManager().pushScene(sceneID);

	auto myPanel = std::make_shared<JEngine::UIPanel>();
	auto myPanelID = engine.getUI().getUIBase().addPanel(myPanel);
	engine.getUI().getUIBase().setCurrentPanel(myPanelID);

	myPanel->elements.push_back(std::make_shared<JEngine::UIElement>());
	myPanel->elements.push_back(std::make_shared<JEngine::UIElement>());

	engine.getUI().getUIBase().setActive(false);


	engine.start();

	engine.cleanUp();

	return 0;
}

//void testJobs()
//{
//	JEngine::Engine::startup();
//	JEngine::Engine & engine = JEngine::Engine::getEngine();
//
//	engine.initialise("Test Game", "log.txt");
//
//	auto sceneID = engine.getSceneManager().registerScene(std::make_shared<TestScene>());
//	engine.getSceneManager().pushScene(sceneID);
//
//
//	std::thread t([&]() {
//		std::vector<std::shared_ptr<TestJob>> jobs(5000000);
//
//		auto time1 = glfwGetTime();
//
//		//Add some test jobs
//		for (int i = 0; i < 5000000; ++i)
//		{
//			jobs[i] = std::make_shared<TestJob>(i + 10000000);
//			engine.getJobManager().enqueueJob(jobs[i]);
//		}
//
//		for (int i = 0; i < 5000000; ++i)
//		{
//			jobs[i]->waitUntilFinished();
//		}
//		auto time2 = glfwGetTime();
//
//		auto len = time2 - time1;
//		JEngine::Logger::getLogger().log(strJoinConvert("Time taken: ", len));
//	});
//
//	t.detach();
//
//
//	engine.start();
//
//
//	engine.cleanUp();
//}