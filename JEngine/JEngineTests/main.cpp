#include <JEngineLib\Game.h>
#include <JEngineLib\Maths.h>
#include <JEngineLib\SceneManager.h>

#include "TestScene.h"


int main()
{
	JEngine::Game::createGame();
	JEngine::Game & game = JEngine::Game::getGame();

	game.initialise("Test Game", "log.txt");

	auto sceneID = game.getSceneManager().registerScene(std::make_shared<TestScene>());
	game.getSceneManager().pushScene(sceneID);
	
	game.start();

	return 0;
}