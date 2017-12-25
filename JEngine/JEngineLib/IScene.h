#pragma once

namespace JEngine
{
	class Engine;

	__interface IScene
	{
		void preSceneRender(Engine &);
		void postSceneRender(Engine &);
	};
}