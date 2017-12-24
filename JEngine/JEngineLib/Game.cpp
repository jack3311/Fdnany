#include "Game.h"

#include "GameTime.h"
#include "Logger.h"
#include "Input.h"

namespace JEngine
{
	void errorCallback(int _error, const char * _description)
	{
		fprintf(stderr, "Error: %s\n", _description);
	}


	Game * Game::game;

	Game::Game() :
		logger(Logger::getLogger())
	{
		//Create game systems
		gameTime = std::make_unique<GameTime>();
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

	const ivec2 & Game::getWindowSize() const
	{
		return windowSize;
	}

	bool Game::start()
	{
		//Initialise GLFW
		if (!glfwInit())
		{
			// Initialization failed
			logger.log("Failed to initialise GLFW");
			return false;
		}

		//Set error callback
		glfwSetErrorCallback(errorCallback);

		//Create window
		window = glfwCreateWindow(1280, 800, "My Title", NULL, NULL);
		if (!window)
		{
			// Window or OpenGL context creation failed
			logger.log("Failed to create window");
			return false;
		}

		//Set current context
		glfwMakeContextCurrent(window);

		//Initialise GLEW
		if (glewInit() != GLEW_OK)
		{
			// Glew loading failed
			logger.log("Failed to initialise GLEW");
			return false;
		}

		//Initialise game systems
		Input::initialise();
		

		//Main loop
		while (!glfwWindowShouldClose(window))
		{
			//Keep running

			doOneGameFrame();

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glfwTerminate();

		return true;
	}

	void Game::doOneGameFrame()
	{
		//PRE-RENDER:
		
		//Get window size
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		windowSize.set_xy(width, height);

		//Set GL viewport
		glViewport(0, 0, width, height);

		//Update systems
		gameTime->update();
		Input::update();




		//RENDER:
		render();




		//POST-RENDER:

	}

	void Game::render() const
	{
		glClearColor(1.f, 0.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}