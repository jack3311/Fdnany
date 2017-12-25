#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "EngineTime.h"

#include "Logger.h"
#include "Util.h"

namespace JEngine
{
#define BASE_FRAMERATE 1.f / 60.f
#define MAX_FRAMES_FOR_FPS_AVG 10000u
#define MAX_SECONDS_FOR_FPS_AVG 5.f

	EngineTime::EngineTime() :
		timeSinceStart(0.f),
		timeSinceStartUnscaled(0.f),
		timeScale(1.f),
		numFramesForFPS(0u),
		cumulativeSecondsForFPS(0.f)
	{
		lastTime = glfwGetTime();
	}


	EngineTime::~EngineTime()
	{
	}

	void EngineTime::update()
	{
		//Calculate delta time
		double currentTime = glfwGetTime();

		deltaTimeUnscaled = static_cast<float>(currentTime - lastTime);

		//Check for breaks
		if (deltaTimeUnscaled > 1.f / 5.f) //5fps
		{
			deltaTimeUnscaled = BASE_FRAMERATE;
		}

		deltaTime = timeScale * deltaTimeUnscaled;

		lastTime = currentTime;

		//Increment time
		timeSinceStart += deltaTime;
		timeSinceStartUnscaled += deltaTimeUnscaled;

		//Update FPS
		++numFramesForFPS;
		cumulativeSecondsForFPS += deltaTimeUnscaled;
		if (numFramesForFPS == MAX_FRAMES_FOR_FPS_AVG || cumulativeSecondsForFPS >= MAX_SECONDS_FOR_FPS_AVG)
		{
			float secondsPerFrame = cumulativeSecondsForFPS / static_cast<float>(numFramesForFPS);
			float framesPerSecond = 1.f / secondsPerFrame;

			Logger::getLogger().log(strJoinConvert("s/f:", secondsPerFrame));
			Logger::getLogger().log(strJoinConvert("f/s:", framesPerSecond));

			numFramesForFPS = 0u;
			cumulativeSecondsForFPS = 0.f;
		}
	}

	void EngineTime::setTimeScale(float _value)
	{
		timeScale = _value;
	}

	float EngineTime::getTimeSinceStart() const
	{
		return timeSinceStart;
	}

	float EngineTime::getTimeSinceStartUnscaled() const
	{
		return timeSinceStartUnscaled;
	}
}