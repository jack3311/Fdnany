#include "TestScene.h"

#include <JEngineLib\JobManagement.h>
#include <JEngineLib\Input.h>
#include <JEngineLib\Logger.h>
#include <JEngineLib\Util.h>
#include <JEngineLib\StackAllocator.h>
#include <JEngineLib\PoolAllocator.h>
#include <JEngineLib\ResourceManagement.h>
#include <JEngineLib\Shader.h>
#include <JEngineLib\Renderer.h>
#include <JEngineLib\ResourceFont.h>
#include <JEngineLib\Maths.h>
#include <JEngineLib\DebugRendering.h>
#include <JEngineLib\View.h>
#include <JEngineLib\EngineTime.h>
#include <JEngineLib\World.h>
#include <JEngineLib\ComponentRenderable.h>

#include "TestJob.h"
#include "SystemTest.h"



#include <iostream>


void MyVertexFormat::setupVertexAttributes()
{
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertexFormat), (GLvoid *)offsetof(MyVertexFormat, MyVertexFormat::position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(MyVertexFormat), (GLvoid *)offsetof(MyVertexFormat, MyVertexFormat::texCoords));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

MyVertexFormat::MyVertexFormat(const vec3 & _pos, const vec2 & _texCoords) : position(_pos), texCoords(_texCoords)
{
}


//class Parent
//{
//public:
//	Parent() : a(6) {}
//
//	int a;
//
//	virtual void print()
//	{
//		std::cout << a << std::endl;
//	}
//};
//class Child1 : public Parent
//{
//public:
//	Child1() : b(5.f) {}
//
//	float b;
//
//	virtual void print()
//	{
//		std::cout << b << std::endl;
//	}
//};
//class Child2 : public Parent
//{
//public:
//	Child2() : c(7.f) {}
//
//	double c;
//
//	virtual void print()
//	{
//		std::cout << c << std::endl;
//	}
//};
//
//struct TS
//{
//	float a, b;
//	TS(float _a, float _b) : a(_a), b(_b) {}
//};

TestScene::TestScene()
{
	//JEngine::Engine::get().getWorld().getSystemManager().registerSystem<SystemTest>();






	JEngine::Input::keyDown += [](int _key) {

	};
	JEngine::Input::keyUp += [](int _key) {
		if (_key == GLFW_KEY_Q)
			JEngine::Engine::get().stop();
	};


	/*
	std::shared_ptr<JEngine::Entity> testTransform = std::make_shared<JEngine::Entity>();

	testTransform->localMove({ 1.f, 1.f, 1.f });
	testTransform->localScale({ 0.5f, 0.5f, 0.5f });
	testTransform->localRotate(angleAxis(2.f, vec3{ 0.f, 1.f, 0.f }));
	testTransform->flush();

	auto child1 = std::make_shared<JEngine::Entity>();
	child1->localMove({ 1.f, 1.f, 1.f });
	child1->flush();

	testTransform->addChild(child1);

	JEngine::Engine::get().getWorld().addChild(testTransform);*/

	//testTransform.updateGlobalTransformMatrixRecursive();









	{
		std::vector<MyVertexFormat> vertices = {
			MyVertexFormat{ vec3{ -0.5f, -0.5f, 1.f }, vec2{ 0.f, 0.f } },
			MyVertexFormat{ vec3{ 0.5f, -0.5f, 1.f }, vec2{ 1.f, 0.f } },
			MyVertexFormat{ vec3{ -0.5f, 0.5f, 1.f }, vec2{ 0.f, 1.f } },
			MyVertexFormat{ vec3{ 0.5f, 0.5f, 1.f }, vec2{ 1.f, 1.f } },
		};

		std::vector<GLuint> indices = {
			0, 1, 2,
			1, 3, 2
		};

		renderer1 = std::make_shared<JEngine::Renderer<MyVertexFormat, true>>();
		renderer1->initialise(vertices, indices);

		JEngine::Logger::get().log("Created quad renderer");
	}

	{
		std::vector<MyVertexFormat> vertices = {
			MyVertexFormat{ vec3{ -0.5f, -0.5f, 1.f }, vec2{ 0.f, 0.f } },
			MyVertexFormat{ vec3{ 0.5f, -0.5f, 1.f }, vec2{ 1.f, 0.f } },
			MyVertexFormat{ vec3{ -0.5f, 0.5f, 1.f }, vec2{ 0.f, 1.f } }
		};

		std::vector<GLuint> indices = {
			0, 1, 2,
		};

		renderer2 = std::make_shared<JEngine::Renderer<MyVertexFormat, true>>();
		renderer2->initialise(vertices, indices);

		JEngine::Logger::get().log("Created triangle renderer");
	}

	{
		rendererTerrain = std::make_shared<JEngine::RendererTerrain>();
		rendererTerrain->initialise();
	}


	
	auto & resourceManager = JEngine::ResourceManager::getResourceManager();

	resourceManager.beginResourceCaching();
	{
		std::shared_ptr<JEngine::JobLoadResourceTexture> job;

		resourceManager.loadResourceTextureAsync(job, "Assets\\image1.png");

		job->waitUntilFinished();
		if (!job->texture->initialise())
		{
			JEngine::Logger::get().log("Could not initialise test texture");
		}
		testTexture = job->texture;
		JEngine::Logger::get().log("Loaded test texture");
	}
	resourceManager.endResourceCaching();
	



	{
		testShader = std::shared_ptr<JEngine::Shader>(new JEngine::Shader());

		auto job = testShader->loadFromDiskAsync({
			{ JEngine::Shader::ShaderComponent::VERTEX, resourceManager.constructFullPath("Assets\\testShader.vert") },
			{ JEngine::Shader::ShaderComponent::FRAGMENT, resourceManager.constructFullPath("Assets\\testShader.frag") }
		});

		job->waitUntilFinished();

		if (!job->wasSuccessful())
		{
			std::cout << "Could not load test shader" << std::endl;
		}

		if (!testShader->initialise())
		{
			std::cout << "Could not initialise test shader" << std::endl;
		}
	}


	{
		terrainShader = std::shared_ptr<JEngine::Shader>(new JEngine::Shader());

		auto job = terrainShader->loadFromDiskAsync({
			{ JEngine::Shader::ShaderComponent::VERTEX, resourceManager.constructFullPath("Assets\\terrainShader.vert") },
			{ JEngine::Shader::ShaderComponent::FRAGMENT, resourceManager.constructFullPath("Assets\\terrainShader.frag") }
			});

		job->waitUntilFinished();

		if (!job->wasSuccessful())
		{
			std::cout << "Could not load terrain shader" << std::endl;
		}

		if (!terrainShader->initialise())
		{
			std::cout << "Could not initialise terrain shader" << std::endl;
		}
	}

	{
		materialTerrain = std::make_shared<JEngine::Material<UniformBufferFormatNull>>();
		materialTerrain->initialise(&*terrainShader);
	}



	resourceManager.beginResourceCaching();
	{
		//Load terrain textures
		std::shared_ptr<JEngine::JobLoadResourceTexture> jobTerrainTex0;
		std::shared_ptr<JEngine::JobLoadResourceTexture> jobTerrainTex1;
		std::shared_ptr<JEngine::JobLoadResourceTexture> jobTerrainTex2;
		std::shared_ptr<JEngine::JobLoadResourceTexture> jobTerrainTex3;

		resourceManager.loadResourceTextureAsync(jobTerrainTex0, "Assets\\wests_textures\\grass1.png");
		resourceManager.loadResourceTextureAsync(jobTerrainTex1, "Assets\\wests_textures\\snow 1.png");
		resourceManager.loadResourceTextureAsync(jobTerrainTex2, "Assets\\wests_textures\\dirt 1.png");
		resourceManager.loadResourceTextureAsync(jobTerrainTex3, "Assets\\wests_textures\\stone 3.png");

		jobTerrainTex0->waitUntilFinished();
		jobTerrainTex1->waitUntilFinished();
		jobTerrainTex2->waitUntilFinished();
		jobTerrainTex3->waitUntilFinished();

		jobTerrainTex0->texture->initialise();
		jobTerrainTex1->texture->initialise();
		jobTerrainTex2->texture->initialise();
		jobTerrainTex3->texture->initialise();


		//Set material textures
		materialTerrain->getTextures()[0] = jobTerrainTex0->texture;
		materialTerrain->getTextures()[1] = jobTerrainTex1->texture;
		materialTerrain->getTextures()[2] = jobTerrainTex2->texture;
		materialTerrain->getTextures()[3] = jobTerrainTex3->texture;
	}
	resourceManager.endResourceCaching();

	//{
	//	textShader = std::shared_ptr<JEngine::Shader>(new JEngine::Shader({
	//		{ JEngine::Shader::ShaderComponent::VERTEX, resourceManager.constructFullPath("Assets\\text.vert") },
	//		{ JEngine::Shader::ShaderComponent::FRAGMENT, resourceManager.constructFullPath("Assets\\text.frag") }
	//	}));
	//
	//	auto job = textShader->loadFromDiskAsync();
	//	job->waitUntilFinished();
	//
	//	if (!job->wasSuccessful())
	//	{
	//		std::cout << "Could not load text shader" << std::endl;
	//	}
	//
	//	if (!textShader->initialise())
	//	{
	//		std::cout << "Could not initialise text shader" << std::endl;
	//	}
	//}




	//testFont = std::make_shared<JEngine::ResourceFont>(JEngine::ResourceManager::getResourceManager().constructFullPath("Assets\\arial.ttf"));
	//if (!testFont->initialise())
	//{
	//	JEngine::Logger::get().log("Could not initialise font");
	//}
	//JEngine::Logger::get().log("Font loaded successfully");
	//
	//
	//rendererText = std::make_shared<JEngine::RendererText>();
	//if (!rendererText->initialise())
	//{
	//	JEngine::Logger::get().log("Could not initialise text renderer");
	//}
	//
	//
	//
	//testCamera = std::make_shared<JEngine::Camera>(JEngine::ProjectionType::ORTHOGRAPHIC, 0.1f, 100.f);
	//testCamera->flush();
	////textShader->setAssociatedCamera(testCamera);





	material1 = std::make_shared<JEngine::Material<UniformBufferFormatNull>>();
	material1->initialise(&*testShader);

	material2 = std::make_shared<JEngine::Material<UniformBufferFormatNull>>();
	material2->initialise(&*testShader);



	JEngine::ECS::EntityManager & entityManager = JEngine::Engine::get().getWorld().getEntityManager();
	JEngine::ECS::ComponentManager & componentManager = JEngine::Engine::get().getWorld().getComponentManager();



	JEngine::ECS::Entity & newEntity = entityManager.createEntity();
	{
		newEntity.localMove({ 1.f, 1.f, 1.f });
		newEntity.localScale({ 3.f, 3.f, 3.f });
		newEntity.localRotate(angleAxis(2.f, vec3{ 0.f, 1.f, 0.f }));
		newEntity.flush();

		componentManager.createComponent<JEngine::ComponentRenderable>(newEntity.getEntityID(), &*material1, &*renderer1);
	}

	JEngine::ECS::Entity & newEntity2 = entityManager.createEntity();
	{
		newEntity2.setParent(&newEntity);

		newEntity2.localMove({ 2.5f, 0.f, 0.f });
		newEntity2.flush();

		componentManager.createComponent<JEngine::ComponentRenderable>(newEntity2.getEntityID(), &*material1, &*renderer2);
	}

	JEngine::ECS::Entity & newEntity3 = entityManager.createEntity();
	{
		newEntity3.setParent(&newEntity2);

		newEntity3.localMove({ 0, -2.5f, 0.f });
		newEntity3.flush();

		componentManager.createComponent<JEngine::ComponentRenderable>(newEntity3.getEntityID(), &*material2, &*renderer1);
	}

	JEngine::ECS::Entity & newEntity4 = entityManager.createEntity();
	{
		newEntity4.setParent(&newEntity3);

		newEntity4.localMove({ 0, -2.5f, 0.f });
		newEntity4.flush();

		componentManager.createComponent<JEngine::ComponentRenderable>(newEntity4.getEntityID(), &*material2, &*renderer1);
	}

	entity1 = &newEntity;

	

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
	//JEngine::Logger::get().log("Job finished");

	/* TEXTURE RESOURCE LOADING
	
	auto & resourceManager = JEngine::ResourceManager::getResourceManager();

	resourceManager.beginResourceCaching();

	std::shared_ptr<JEngine::JobLoadResourceTexture> job, job1, job2, job3;

	resourceManager.loadResourceTextureAsync(job, "Assets\\image1.png");

	resourceManager.loadResourceTextureAsync(job1, "Assets\\image2.png");

	resourceManager.loadResourceTextureAsync(job2, "Assets\\image3.png");

	resourceManager.loadResourceTextureAsync(job3, "Assets\\image1.png");

	job->waitUntilFinished();
	JEngine::Logger::get().log("Loaded texture 0");
	job->texture->initialise();

	job1->waitUntilFinished();
	JEngine::Logger::get().log("Loaded texture 1");
	job1->texture->initialise();

	job2->waitUntilFinished();
	JEngine::Logger::get().log("Loaded texture 2");
	job2->texture->initialise();

	job3->waitUntilFinished();
	JEngine::Logger::get().log("Loaded texture 3");
	job3->texture->initialise();

	resourceManager.endResourceCaching();*/


	/* JOBS/THREADING TESTS
	static int currentNum = 0;
	if (_engine.getJobManager().getJobCount() < _engine.getJobManager().getNumWorkers())
	{
		auto job = std::make_shared<TestJob>(++currentNum);

		job->getEvent() += [](const JEngine::Job * _job) {
			auto job = dynamic_cast<const TestJob *>(_job);
			JEngine::Logger::get().log(strJoinConvert(job->num, " result: ", job->isThisNumberPrimeData, " from thread: ", std::this_thread::get_id()));
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
	JEngine::Logger::get().log(strJoinConvert("Time taken: ", len));
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
	JEngine::Logger::get().log(strJoinConvert("Time taken: ", len));
	*/

	float time = _engine.getEngineTime().getTimeSinceStart();

	_engine.getCurrentView().getCamera()->localSetPosition({ sinf(time * 0.1f) * 8.f, 4.f, cosf(time * 0.1f) * 8.f });
	
	auto dir = glm::vec3{ 0.f, 0.f, 0.f } -_engine.getCurrentView().getCamera()->getLocalPosition();
	_engine.getCurrentView().getCamera()->localLookAt(dir);
	
	_engine.getCurrentView().getCamera()->flush();


	
	entity1->localRotate(glm::rotate(glm::quat(), _engine.getEngineTime().deltaTime * 3.14f * 0.1f, glm::vec3{ 1, 1, 0 }));
	entity1->flush();




	//Setup test jobs
	/*auto aggregate = std::make_shared<JEngine::JobAggregate>();

	auto aggregate2 = std::make_shared<JEngine::JobAggregate>();

	aggregate2->addJob(std::make_shared<TestJob>(500));
	aggregate2->addJob(std::make_shared<TestJob>(600));
	aggregate2->addJob(std::make_shared<TestJob>(700));

	aggregate->addJob(std::make_shared<TestJob>(100));
	aggregate->addJob(std::make_shared<TestJob>(200));
	aggregate->addJob(aggregate2);
	aggregate->addJob(std::make_shared<TestJob>(300));
	aggregate->addJob(std::make_shared<TestJob>(400));


	auto time1 = glfwGetTime();

	_engine.getJobManager().enqueueJob(aggregate);

	aggregate->waitUntilFinished();

	auto time2 = glfwGetTime();

	auto len = time2 - time1;
	JEngine::Logger::get().log(JEngine::strJoinConvert("Time taken: ", len));

	JEngine::Logger::get().log("Done");*/
}

void TestScene::postSceneRender(JEngine::Engine & _engine)
{
	JEngine::ECS::Entity t(-1);
	t.localSetScale(vec3{ 1, 5, 1 });
	t.localSetPosition({ -50, -20, -50 });
	t.flush();
	t.updateGlobalTransformMatrixRecursive();

	materialTerrain->begin();
	{
		rendererTerrain->begin();
		{
			terrainShader->setTransformUniforms(t);

			rendererTerrain->draw();
		}
		rendererTerrain->end();
	}

	/*
	testShader->begin();

	testShader->setTransformUniforms(*entity1);
	
	testTexture->bind(GL_TEXTURE0);

	{ 
		EnableBlend;
		renderer->draw();
	}
	
	testShader->end();*/

	/*textShader->begin();
	
	rendererText->draw(*testFont, vec2(100, 100), 1.f, "Test text");
	
	textShader->end();*/

	

	//Draw a line
	//for (int offset = 0; offset < 5; ++offset)
	//{
	//	auto px = offset / 20.f;
	//
	//	JEngine::DebugRendering::get().drawLine({
	//		{ { -0.5f + px, -0.5f, 1.f },{ 1.f, 0.f, 0.f } },
	//		{ { 0.5f + px, -0.5f, 1.f },{ 0.f, 1.f, 0.f } },
	//		{ { 0.5f + px, 0.5f, 1.f },{ 0.f, 0.f, 1.f } },
	//		{ { -0.5f + px, 0.5f, 1.f },{ 1.f, 1.f, 1.f } },
	//	});
	//}


	
}