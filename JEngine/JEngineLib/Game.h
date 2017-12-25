#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "Maths.h"

#include <memory>
#include <string>

namespace JEngine
{
	class GameTime;
	class SceneManager;

	class Game
	{
	private:
		static Game * game;
	public:
		static Game & getGame();
		static void createGame();


	private:
		Game();
		~Game();
		Game(const Game &) = delete;

		GLFWwindow * window = nullptr;

		std::unique_ptr<GameTime> gameTime;
		std::unique_ptr<SceneManager> sceneManager;

		ivec2 windowSize;

		bool shouldQuit = false;


		//Frame functions
		void doOneGameFrame();

		void render() const;

		void cleanUp();

	public:


		GameTime & getGameTime();
		SceneManager & getSceneManager();

		const ivec2 & getWindowSize() const;

		bool initialise(std::string _title, std::string _logFile);

		void start();

		void stop();




	};
}