#pragma once

#include <memory>
#include <stack>
#include <vector>

#include "IScene.h"

namespace JEngine
{
	class SceneManager
	{
	private:
		std::stack<std::shared_ptr<IScene>> currentScenes;
		std::vector<std::shared_ptr<IScene>> scenes;

	public:
		SceneManager();
		SceneManager(const SceneManager &) = delete;
		~SceneManager();

		bool initialise();

		unsigned int registerScene(const std::shared_ptr<IScene> & _scene);

		///
		/// pushScene:
		/// adds a registered scene to the stack
		/// _scene: the scene ID
		///
		void pushScene(unsigned int _scene);

		///
		/// pushScene:
		/// adds a temporary scene to the stack
		/// _scene: the scene
		///
		void pushScene(const std::shared_ptr<IScene> & _scene);

		///
		/// popScene:
		/// pops the current scene off the stack
		///
		void popScene();

		std::shared_ptr<IScene> getCurrentScene();

		bool hasNoScenes() const;
	};
}
