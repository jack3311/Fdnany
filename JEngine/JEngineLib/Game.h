#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "Maths.h"

#include <memory>

namespace JEngine
{
	class GameTime;
	class Logger;

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

		Logger & logger;

		ivec2 windowSize;

		//Frame functions
		void doOneGameFrame();

		void render() const;

	public:


		GameTime & getGameTime();
		const ivec2 & getWindowSize() const;



		bool start();



	};
}