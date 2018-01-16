#include "Engine.h"

#include <iostream>

#include "EngineTime.h"
#include "SceneManagement.h"
#include "JobManagement.h"
#include "StackAllocator.h"
#include "UI.h"
#include "ResourceManagement.h"
#include "Logger.h"
#include "Input.h"
#include "Util.h"
#include "Camera.h"
#include "View.h"

namespace JEngine
{
#define DEFAULT_FOV glm::pi<float>() * 0.5f
#define DEFAULT_Z_NEAR 0.1f
#define DEFAULT_Z_FAR 100.f

	namespace
	{
		void errorCallbackGLFW(int _error, const char * _description)
		{
			Logger::getLogger().log(_description, LogLevel::ERROR);
			Engine::getEngine().stop();
		}
		void APIENTRY errorCallbackGL(GLenum _source, GLenum _type, GLuint _id, GLenum _severity, 
			GLsizei _length, const GLchar * _message, const void * _userParam)
		{
			Logger::getLogger().log(strJoin({ "GL: ", _message }), LogLevel::ERROR);
		}
	}


	Engine * Engine::engine;

	Engine::Engine()
	{
		//Create engine systems
		engineTime = std::make_unique<EngineTime>();
		sceneManager = std::make_unique<SceneManager>();
		jobManager = std::make_unique<JobManager>();
		frameAllocator = std::make_unique<StackAllocator>();
		ui = std::make_unique<UI>();
		standardView = std::make_unique<View>(
			std::make_shared<Camera>(ProjectionType::PERSPECTIVE, DEFAULT_FOV, DEFAULT_Z_NEAR, DEFAULT_Z_FAR)
		);
	}

	Engine::~Engine()
	{
	}

	Engine & Engine::getEngine()
	{
		return *engine;
	}

	void Engine::create()
	{
		engine = new Engine();
	}

	EngineTime & Engine::getEngineTime()
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

	StackAllocator & Engine::getFrameAllocator()
	{
		return *frameAllocator;
	}

	UI & Engine::getUI()
	{
		return *ui;
	}

	View & Engine::getView()
	{
		return *standardView;
	}

	const ivec2 & Engine::getWindowSizeInt() const
	{
		return windowSize;
	}

	const vec2 Engine::getWindowSizeFloat() const
	{
		return static_cast<vec2>(windowSize);
	}

	bool Engine::initialise(std::string _title, std::string _logFile, const ivec2 & _windowSize)
	{
		//Misc
		mainThreadID = std::this_thread::get_id();

		//Initialise Logger
		Logger::create();
		ERR_IF(!Logger::getLogger().initialise("log.txt"), "Failed to initialise logger");

		//Initialise GLFW
		ERR_IF(!glfwInit(), "Failed to initialise GLFW");
		
		//Set GLFW error callback
		glfwSetErrorCallback(errorCallbackGLFW);

		//Create window
		windowSize = _windowSize;
		window = glfwCreateWindow(_windowSize.x, _windowSize.y, "My Title", NULL, NULL);
		ERR_IF(!window, "Failed to create window");

		//Set current context
		glfwMakeContextCurrent(window);

		//Initialise GLEW
		ERR_IF(glewInit() != GLEW_OK, "Failed to initialise GLEW");

		//Set OpenGL error callback
#if _DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(errorCallbackGL, nullptr);
#endif

		//OpenGL config
		glEnable(GL_TEXTURE_2D);
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);

		//Initialise engine systems
		ERR_IF(!standardView->initialise(), "Failed to initialise standard view");
		ERR_IF(!Input::initialise(window), "Failed to initialise input system");
		ERR_IF(!sceneManager->initialise(), "Failed to initialise scene manager");
		ERR_IF(!jobManager->initialise(), "Failed to initialise job manager");
		ERR_IF(!frameAllocator->initialise(1000000), "Failed to initialise frame allocator");
		ERR_IF(!ResourceManager::create().initialise(), "Failed to initialise resource manager");
		ERR_IF(!ui->initialise(), "Failed to initialise UI");

		//Setup blocking input events
		Input::keyDown += [this](int _key) { keyDownBlockable.triggerEvent(_key); };
		Input::keyUp += [this](int _key) { keyUpBlockable.triggerEvent(_key); };
		Input::mouseDown += [this](int _key) { mouseDownBlockable.triggerEvent(_key); };
		Input::mouseUp += [this](int _key) { mouseUpBlockable.triggerEvent(_key); };
		

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

	const std::thread::id & Engine::getMainThreadID() const
	{
		return mainThreadID;
	}

	bool Engine::isCurrentThreadMain() const
	{
		return std::this_thread::get_id() == mainThreadID;
	}

	void Engine::cleanUp()
	{
		//Clean up/Shut down engine systems
		ResourceManager::getResourceManager().cleanUp();
		frameAllocator->cleanUp();
		jobManager->stop();

		Logger::getLogger().cleanUp();

		glfwTerminate();
	}

	void Engine::executeOneFrame()
	{
		//PRE-RENDER:

		//Get current scene
		if (sceneManager->hasNoCurrentScenes())
		{
			//No scene, stop engine
			stop();
			return;
		}
		auto currentScene = sceneManager->getCurrentScene();
		
		//Get window size
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		windowSize = { width, height };

		//Set GL viewport
		glViewport(0, 0, width, height);

		//Update systems
		engineTime->update();
		Input::update();
		ui->update();

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

		ui->render();
	}
}