#pragma once

namespace JEngine
{
	class Engine;

	class Scene
	{
	public:
		Scene();
		Scene(const Scene &) = delete;
		~Scene();

		virtual void preSceneRender(Engine &) = 0;
		virtual void postSceneRender(Engine &) = 0;
	};
}