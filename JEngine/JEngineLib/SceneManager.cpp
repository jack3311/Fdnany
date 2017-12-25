#include "SceneManager.h"

#include <cassert>

#include "Logger.h"
#include "Util.h"

namespace JEngine
{
	SceneManager::SceneManager()
	{
	}

	SceneManager::~SceneManager()
	{
	}

	bool SceneManager::initialise()
	{
		return true;
	}

	unsigned int SceneManager::registerScene(const std::shared_ptr<IScene> & _scene)
	{
		scenes.push_back(_scene);
		
		Logger::getLogger().log(strJoin({ "Registered scene: ", typeid(*_scene).name() }));

		return static_cast<unsigned int>(scenes.size()) - 1u;
	}

	void SceneManager::pushScene(unsigned int _scene)
	{
		assert(0u <= _scene);
		assert(_scene < scenes.size());

		auto scene = scenes[_scene];

		currentScenes.push(scenes[_scene]);

		Logger::getLogger().log(strJoin({ "Pushed static scene: ", typeid(*scene).name() }));
	}

	void SceneManager::pushScene(const std::shared_ptr<IScene> & _scene)
	{
		currentScenes.push(_scene);

		Logger::getLogger().log(strJoin({ "Pushed instanced scene: ", typeid(*_scene).name() }));
	}

	void SceneManager::popScene()
	{
		assert(currentScenes.size() > 0);

		auto topScene = currentScenes.top();
		currentScenes.pop();
		
		Logger::getLogger().log(strJoin({ "Popped scene: ", typeid(*topScene).name() }));
	}

	std::shared_ptr<IScene> SceneManager::getCurrentScene()
	{
		return currentScenes.top();
	}

	bool SceneManager::hasScene() const
	{
		return currentScenes.size() > 0u;
	}
}