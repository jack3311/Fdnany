#pragma once

namespace JEngine
{
	class Game;

	__interface IScene
	{
		void initialise(Game);
		void preSceneRender(Game);
		void postSceneRender(Game);
	};
}