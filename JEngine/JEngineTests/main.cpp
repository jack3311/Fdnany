#include "JEngineLib\Game.h"
#include "JEngineLib\Maths.h"


int main()
{
	JEngine::Game::createGame();
	JEngine::Game & game = JEngine::Game::getGame();
	bool res = game.start();

	return res;
}