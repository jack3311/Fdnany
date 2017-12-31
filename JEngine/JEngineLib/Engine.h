#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "Maths.h"
#include "JEvent.h"

#include <memory>
#include <string>
#include <mutex>

namespace JEngine
{
	class EngineTime;
	class SceneManager;
	class JobManager;
	class FrameAllocator;
	class UI;

	class Engine
	{
	private:
		static Engine * engine;
	public:
		static Engine & getEngine();
		static void startup();


	private:
		Engine();
		~Engine();
		Engine(const Engine &) = delete;

		GLFWwindow * window = nullptr;

		std::unique_ptr<EngineTime> engineTime;
		std::unique_ptr<SceneManager> sceneManager;
		std::unique_ptr<JobManager> jobManager;
		std::unique_ptr<FrameAllocator> frameAllocator;
		std::unique_ptr<UI> ui;

		ivec2 windowSize;

		bool shouldQuit = false;


		//Frame functions
		void executeOneFrame();

		void render() const;

	public:
		EngineTime & getEngineTime();
		SceneManager & getSceneManager();
		JobManager & getJobManager();
		FrameAllocator & getFrameAllocator();
		UI & getUI();

		JEventBlockable<int> mouseDownBlockable, mouseUpBlockable;
		JEventBlockable<int> keyDownBlockable, keyUpBlockable;

		const ivec2 & getWindowSize() const;

		bool initialise(std::string _title, std::string _logFile);

		void cleanUp();


		void start();
		void stop();
	};
}