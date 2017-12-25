#include "Game.h"

#include <iostream>

#include "GameTime.h"
#include "SceneManager.h"
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
			Game::getGame().stop();
		}
	}


	Game * Game::game;

	Game::Game()
	{
		//Create game systems
		gameTime = std::make_unique<GameTime>();
		sceneManager = std::make_unique<SceneManager>();
	}

	Game::~Game()
	{
	}

	Game & Game::getGame()
	{
		return *game;
	}

	void Game::createGame()
	{
		game = new Game();
	}

	GameTime & Game::getGameTime()
	{
		return *gameTime;
	}

	SceneManager & Game::getSceneManager()
	{
		return *sceneManager;
	}

	const ivec2 & Game::getWindowSize() const
	{
		return windowSize;
	}

	bool Game::initialise(std::string _title, std::string _logFile)
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

		//Initialise game systems
		ERR_IF(!Input::initialise(), "Failed to initialise input system");
		ERR_IF(!sceneManager->initialise(), "Failed to initialise scene manager");

		Logger::getLogger().log("Game initialised successfully");

		return true;
	}

	void Game::start()
	{
		//Main loop
		while (!(glfwWindowShouldClose(window) || shouldQuit))
		{
			//Keep running

			doOneGameFrame();

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		//Clean up resources
		cleanUp();
	}

	void Game::stop()
	{
		shouldQuit = true;
	}

	void Game::cleanUp()
	{
		glfwTerminate();
	}

	void Game::doOneGameFrame()
	{
		//PRE-RENDER:

		//Get current scene
		if (!sceneManager->hasScene())
		{
			//No scene, stop game
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
		gameTime->update();
		Input::update();

		//Pre-render for current scene
		currentScene->preSceneRender(*this);


		//RENDER:
		render();


		//POST-RENDER:

		//Post-render for current scene
		currentScene->postSceneRender(*this);

	}

	void Game::render() const
	{
		glClearColor(1.f, 0.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}