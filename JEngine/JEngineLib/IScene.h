#pragma once

namespace JEngine
{
	class Game;

	__interface IScene
	{
		void preSceneRender(Game &);
		void postSceneRender(Game &);
	};
}