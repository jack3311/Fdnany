#include "TestScene.h"

#include <JEngineLib\JobManagement.h>
#include <JEngineLib\Input.h>
#include <JEngineLib\Logger.h>
#include <JEngineLib\Util.h>
#include <JEngineLib\FrameAllocator.h>
#include <JEngineLib\PoolAllocator.h>

#include "TestJob.h"


#include <iostream>



TestScene::TestScene()
{
}


TestScene::~TestScene()
{
}

struct TS
{
	float a, b;
	TS(float _a, float _b) : a(_a), b(_b) {}
};

void TestScene::preSceneRender(JEngine::Engine & _engine)
{
	/* JOBS/THREADING TESTS
	static int currentNum = 0;
	if (_engine.getJobManager().getJobCount() < _engine.getJobManager().getNumWorkers())
	{
		auto job = std::make_shared<TestJob>(++currentNum);

		job->getEvent() += [](const JEngine::Job * _job) {
			auto job = dynamic_cast<const TestJob *>(_job);
			JEngine::Logger::getLogger().log(strJoinConvert(job->num, " result: ", job->isThisNumberPrimeData, " from thread: ", std::this_thread::get_id()));
		};

		_engine.getJobManager().enqueueJob(job);
	}
	*/

	/* FRAME ALLOCATION TESTS
	double * doubles[100000];

	auto time1 = glfwGetTime();
	{
		for (int i = 0; i < 100; ++i)
		{
			for (int i = 0; i < 100000; ++i)
			{
				doubles[i] = _engine.getFrameAllocator().allocateRaw<double>();
				//doubles[i] = new double;
			}


			//for (int i = 0; i < 100000; ++i)
			//{
			//	delete doubles[i];
			//}

			_engine.getFrameAllocator().reset();
		}
	}
	auto time2 = glfwGetTime();

	auto len = time2 - time1;
	JEngine::Logger::getLogger().log(strJoinConvert("Time taken: ", len));
	*/

	/* Pool Allocator tests */
	JEngine::RcPoolAllocator<sizeof(float), 21> rcPoolAllocator;
	JEngine::pool_alloc_pointer<float, sizeof(float), 21> floats[10];
	JEngine::pool_alloc_pointer<int, sizeof(float), 21> ints[10];
	double * doublesP[10];

	rcPoolAllocator.initialise();

	srand(0u);

	auto time1 = glfwGetTime();
	{
		for (int i = 0; i < 1000; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				if (rand() % 2 == 0)
				{
					floats[j] = rcPoolAllocator.allocate<float>(13.37f);
				}
				else
				{
					ints[j] = rcPoolAllocator.allocate<int>(11);
				}
				//doublesP[j] = new double;
			}

			//for (int j = 0; j < 10; ++j)
			//{
			//	delete doublesP[j];
			//}
		}
	}

	for (int i = 0; i < 10; ++i)
	{
		std::cout << *floats[i] << std::endl;
	}
	for (int i = 0; i < 10; ++i)
	{
		std::cout << *ints[i] << std::endl;
	}

	auto time2 = glfwGetTime();
	rcPoolAllocator.cleanUp();

	auto len = time2 - time1;
	JEngine::Logger::getLogger().log(strJoinConvert("Time taken: ", len));
	
	
}

void TestScene::postSceneRender(JEngine::Engine & _engine)
{
	if (JEngine::Input::isKeyDown(GLFW_KEY_Q) == JEngine::KeyState::KEY_PRESSED)
	{
		_engine.stop();
	}
}
