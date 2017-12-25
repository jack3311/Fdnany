#pragma once
#include <JEngineLib\IScene.h>
#include <JEngineLib\Game.h>

class TestScene :
	public JEngine::IScene
{
public:
	TestScene();
	~TestScene();

	void preSceneRender(JEngine::Game &);
	void postSceneRender(JEngine::Game &);
};

