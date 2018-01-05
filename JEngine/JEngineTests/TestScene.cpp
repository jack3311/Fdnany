#include "TestScene.h"

#include <JEngineLib\JobManagement.h>
#include <JEngineLib\Input.h>
#include <JEngineLib\Logger.h>
#include <JEngineLib\Util.h>
#include <JEngineLib\StackAllocator.h>
#include <JEngineLib\PoolAllocator.h>
#include <JEngineLib\ResourceManagement.h>

#include "TestJob.h"


#include <iostream>


class Parent
{
public:
	Parent() : a(6) {}

	int a;

	virtual void print()
	{
		std::cout << a << std::endl;
	}
};
class Child1 : public Parent
{
public:
	Child1() : b(5.f) {}

	float b;

	virtual void print()
	{
		std::cout << b << std::endl;
	}
};
class Child2 : public Parent
{
public:
	Child2() : c(7.f) {}

	double c;

	virtual void print()
	{
		std::cout << c << std::endl;
	}
};

struct TS
{
	float a, b;
	TS(float _a, float _b) : a(_a), b(_b) {}
};

TestScene::TestScene()
{
	JEngine::Input::keyDown += [](int _key) {

	};
	JEngine::Input::keyUp += [](int _key) {
		if (_key == GLFW_KEY_Q)
			JEngine::Engine::getEngine().stop();
	};
}

TestScene::~TestScene()
{
}

void TestScene::preSceneRender(JEngine::Engine & _engine)
{
	//auto job = std::make_shared<JEngine::JobLoadResourceTexture>(
	//	JEngine::ResourceManager::getResourceManager().constructFullPath("Assets\\grassblades.png"));
	//_engine.getJobManager().enqueueJob(job);
	//job->waitUntilFinished();
	//job->texture->initialise();
	//JEngine::Logger::getLogger().log("Job finished");

	/* TEXTURE RESOURCE LOADING
	
	auto & resourceManager = JEngine::ResourceManager::getResourceManager();

	resourceManager.beginResourceCaching();

	std::shared_ptr<JEngine::JobLoadResourceTexture> job, job1, job2, job3;

	resourceManager.loadResourceTextureAsync(job, "Assets\\image1.png");

	resourceManager.loadResourceTextureAsync(job1, "Assets\\image2.png");

	resourceManager.loadResourceTextureAsync(job2, "Assets\\image3.png");

	resourceManager.loadResourceTextureAsync(job3, "Assets\\image1.png");

	job->waitUntilFinished();
	JEngine::Logger::getLogger().log("Loaded texture 0");
	job->texture->initialise();

	job1->waitUntilFinished();
	JEngine::Logger::getLogger().log("Loaded texture 1");
	job1->texture->initialise();

	job2->waitUntilFinished();
	JEngine::Logger::getLogger().log("Loaded texture 2");
	job2->texture->initialise();

	job3->waitUntilFinished();
	JEngine::Logger::getLogger().log("Loaded texture 3");
	job3->texture->initialise();

	resourceManager.endResourceCaching();*/


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

	/* Pool Allocator tests 
	JEngine::RcPoolAllocator<sizeof(Child1), 21> rcPoolAllocator;
	//JEngine::pool_alloc_pointer<float, sizeof(float), 21> floats[10];
	//JEngine::pool_alloc_pointer<int, sizeof(float), 21> ints[10];
	JEngine::pool_alloc_pointer<Parent, sizeof(Child1), 21> instances[10];
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
					instances[j] = rcPoolAllocator.allocate<Child1>();
				}
				else
				{
					instances[j] = rcPoolAllocator.allocate<Child2>();
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
		(*instances[i]).print();
	}

	auto time2 = glfwGetTime();
	rcPoolAllocator.cleanUp();

	auto len = time2 - time1;
	JEngine::Logger::getLogger().log(strJoinConvert("Time taken: ", len));
	*/
	
}

void TestScene::postSceneRender(JEngine::Engine & _engine)
{
}
