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
	class StackAllocator;
	class UI;
	class Camera;

	class Engine
	{
	private:
		static Engine * engine;
	public:
		static Engine & getEngine();
		static void create();


	private:
		Engine();
		~Engine();
		Engine(const Engine &) = delete;

		GLFWwindow * window = nullptr;

		std::unique_ptr<EngineTime> engineTime;
		std::unique_ptr<SceneManager> sceneManager;
		std::unique_ptr<JobManager> jobManager;
		std::unique_ptr<StackAllocator> frameAllocator;
		std::unique_ptr<UI> ui;
		std::unique_ptr<Camera> camera;

		ivec2 windowSize;

		bool shouldQuit = false;

		std::thread::id mainThreadID;


		//Frame functions
		void executeOneFrame();

		void render() const;

	public:
		EngineTime & getEngineTime();
		SceneManager & getSceneManager();
		JobManager & getJobManager();
		StackAllocator & getFrameAllocator();
		UI & getUI();
		Camera & getCamera();

		JEventBlockable<int> mouseDownBlockable, mouseUpBlockable;
		JEventBlockable<int> keyDownBlockable, keyUpBlockable;

		const ivec2 & getWindowSizeInt() const;
		const vec2 getWindowSizeFloat() const;

		bool initialise(std::string _title, std::string _logFile, const ivec2 & _windowSize);

		void cleanUp();


		void start();
		void stop();

		const std::thread::id & getMainThreadID() const;
		bool isCurrentThreadMain() const;
	};
}