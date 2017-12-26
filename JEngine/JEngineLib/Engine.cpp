#include "Engine.h"

#include <iostream>

#include "EngineTime.h"
#include "SceneManagement.h"
#include "JobManagement.h"
#include "FrameAllocator.h"
#include "Logger.h"
#include "Input.h"
#include "Util.h"

namespace JEngine
{
	namespace
	{
		void errorCallback(int _error, const char * _description)
		{
			Logger::getLogger().log(_description);
			Engine::getEngine().stop();
		}
	}


	Engine * Engine::engine;

	Engine::Engine()
	{
		//Create engine systems
		engineTime = std::make_unique<EngineTime>();
		sceneManager = std::make_unique<SceneManager>();
		jobManager = std::make_unique<JobManager>();
		frameAllocator = std::make_unique<FrameAllocator>();
	}

	Engine::~Engine()
	{
	}

	Engine & Engine::getEngine()
	{
		return *engine;
	}

	void Engine::startup()
	{
		engine = new Engine();
	}

	EngineTime & Engine::getGameTime()
	{
		return *engineTime;
	}

	SceneManager & Engine::getSceneManager()
	{
		return *sceneManager;
	}

	JobManager & Engine::getJobManager()
	{
		return *jobManager;
	}

	FrameAllocator & Engine::getFrameAllocator()
	{
		return *frameAllocator;
	}

	const ivec2 & Engine::getWindowSize() const
	{
		return windowSize;
	}

	bool Engine::initialise(std::string _title, std::string _logFile)
	{
		//Initialise Logger
		ERR_IF(!Logger::getLogger().initialise("log.txt"), "Failed to initialise logger");

		//Initialise GLFW
		ERR_IF(!glfwInit(), "Failed to initialise GLFW");
		
		//Set error callback
		glfwSetErrorCallback(errorCallback);

		//Create window
		window = glfwCreateWindow(1280, 800, "My Title", NULL, NULL);
		ERR_IF(!window, "Failed to create window");

		//Set current context
		glfwMakeContextCurrent(window);

		//Initialise GLEW
		ERR_IF(glewInit() != GLEW_OK, "Failed to initialise GLEW");

		//Initialise engine systems
		ERR_IF(!Input::initialise(window), "Failed to initialise input system");
		ERR_IF(!sceneManager->initialise(), "Failed to initialise scene manager");
		ERR_IF(!jobManager->initialise(), "Failed to initialise job manager");
		ERR_IF(!frameAllocator->initialise(1000000), "Failed to initialise frame allocator");\

		Logger::getLogger().log("Engine initialised successfully");

		return true;
	}

	void Engine::start()
	{
		//Set current context on the new thread
		glfwMakeContextCurrent(window);

		//Main loop
		while (!(glfwWindowShouldClose(window) || shouldQuit)) //Keep running
		{
			executeOneFrame();

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	void Engine::stop()
	{
		shouldQuit = true;
	}

	void Engine::cleanUp()
	{
		//Clean up/Shut down engine systems
		frameAllocator->cleanUp();
		jobManager->stop();

		Logger::getLogger().cleanUp();

		glfwTerminate();
	}

	void Engine::executeOneFrame()
	{
		//PRE-RENDER:

		//Get current scene
		if (sceneManager->hasNoScenes())
		{
			//No scene, stop engine
			stop();
			return;
		}
		auto currentScene = sceneManager->getCurrentScene();
		
		//Get window size
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		windowSize.set_xy(width, height);

		//Set GL viewport
		glViewport(0, 0, width, height);

		//Update systems
		engineTime->update();
		Input::update();

		//Pre-render for current scene
		currentScene->preSceneRender(*this);


		//RENDER:
		render();


		//POST-RENDER:

		//Post-render for current scene
		currentScene->postSceneRender(*this);

		//Reset frame allocator
		frameAllocator->reset();
	}

	void Engine::render() const
	{
		glClearColor(1.f, 0.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}