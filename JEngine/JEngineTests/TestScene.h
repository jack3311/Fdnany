#pragma once
#include <JEngineLib\IScene.h>
#include <JEngineLib\Engine.h>
#include <JEngineLib\Shader.h>
#include <JEngineLib\Renderer.h>
#include <JEngineLib\Maths.h>

struct MyVertexFormat
{
	JEngine::fvec3 position;

	static void setupVertexAttributes();

	MyVertexFormat(JEngine::fvec3);
};

class TestScene :
	public JEngine::Scene
{
private:
	std::shared_ptr<JEngine::Shader> testShader;
	std::shared_ptr<JEngine::Renderer<MyVertexFormat, true>> renderer;

public:
	TestScene();
	~TestScene();

	void preSceneRender(JEngine::Engine &);
	void postSceneRender(JEngine::Engine &);
};

