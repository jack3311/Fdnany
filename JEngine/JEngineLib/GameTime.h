#pragma once

namespace JEngine
{
	class GameTime
	{
	private:
		double lastTime;

		float timeScale;

		float timeSinceStart;
		float timeSinceStartUnscaled;


	public:
		GameTime();
		~GameTime();

		float deltaTime;
		float deltaTimeUnscaled;

		void update();

		void setTimeScale(float _value);

		float getTimeSinceStart() const;
		float getTimeSinceStartUnscaled() const;
	};
}