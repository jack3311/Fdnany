#include "TestScene.h"

#include <JEngineLib\JobManagement.h>
#include <JEngineLib\Input.h>

#include "TestJob.h"



TestScene::TestScene()
{
}


TestScene::~TestScene()
{
}

void TestScene::preSceneRender(JEngine::Engine & _engine)
{
	static int currentNum = 0;
	if (_engine.getJobManager().getJobCount() < _engine.getJobManager().getNumWorkers())
	{
		_engine.getJobManager().enqueueJob(std::make_shared<TestJob>(++currentNum));
	}
}

void TestScene::postSceneRender(JEngine::Engine & _engine)
{
	if (JEngine::Input::isKeyDown(GLFW_KEY_Q) == JEngine::KeyState::KEY_PRESSED)
	{
		_engine.stop();
	}
}
