#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "GameTime.h"

namespace JEngine
{
	GameTime::GameTime() :
		timeSinceStart(0.f),
		timeSinceStartUnscaled(0.f),
		timeScale(1.f)
	{
		lastTime = glfwGetTime();
	}


	GameTime::~GameTime()
	{
	}

	void GameTime::update()
	{
		//Calculate delta time
		double currentTime = glfwGetTime();

		deltaTimeUnscaled = currentTime - lastTime;
		deltaTime = timeScale * deltaTimeUnscaled;

		lastTime = currentTime;

		//Increment time
		timeSinceStart += deltaTime;
		timeSinceStartUnscaled += deltaTimeUnscaled;
	}

	void GameTime::setTimeScale(float _value)
	{
		timeScale = _value;
	}

	float GameTime::getTimeSinceStart() const
	{
		return timeSinceStart;
	}

	float GameTime::getTimeSinceStartUnscaled() const
	{
		return timeSinceStartUnscaled;
	}
}