#pragma once
#include <JEngineLib\IScene.h>
#include <JEngineLib\Engine.h>

class TestScene :
	public JEngine::Scene
{
public:
	TestScene();
	~TestScene();

	void preSceneRender(JEngine::Engine &);
	void postSceneRender(JEngine::Engine &);
};

