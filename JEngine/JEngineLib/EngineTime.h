#pragma once

namespace JEngine
{
	class EngineTime
	{
	private:
		double lastTime;

		float timeScale;

		float timeSinceStart;
		float timeSinceStartUnscaled;

		unsigned int numFramesForFPS;
		float cumulativeSecondsForFPS;

		float lastSpf, lastFps;


	public:
		EngineTime();
		~EngineTime();

		float deltaTime;
		float deltaTimeUnscaled;

		void update();

		void setTimeScale(float _value);

		float getTimeSinceStart() const;
		float getTimeSinceStartUnscaled() const;

		float getSpf() const;
		float getFps() const;
	};
}