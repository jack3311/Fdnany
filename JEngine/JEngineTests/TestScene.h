#pragma once
#include <JEngineLib\IScene.h>
#include <JEngineLib\Engine.h>
#include <JEngineLib\Shader.h>
#include <JEngineLib\Renderer.h>
#include <JEngineLib\Maths.h>
#include <JEngineLib\ResourceFont.h>
#include <JEngineLib\RendererText.h>
#include <JEngineLib\Camera.h>
#include <JEngineLib\Entity.h>
#include <JEngineLib\Material.h>

struct MyVertexFormat
{
	vec3 position;
	vec2 texCoords;

	static void setupVertexAttributes();

	MyVertexFormat(const vec3 &, const vec2 &);
};

class TestScene :
	public JEngine::Scene
{
private:
	std::shared_ptr<JEngine::Shader> testShader;
	std::shared_ptr<JEngine::Renderer<MyVertexFormat, true>> renderer;

	std::shared_ptr<JEngine::Shader> textShader;
	std::shared_ptr<JEngine::ResourceFont> testFont;
	std::shared_ptr<JEngine::RendererText> rendererText;

	std::shared_ptr<JEngine::Camera> testCamera;

	std::shared_ptr<JEngine::ResourceTexture> testTexture;

	JEngine::ECS::Entity * entity1;

	std::shared_ptr<JEngine::Material> * material1;
	std::shared_ptr<JEngine::Material> * material1;

public:
	TestScene();
	~TestScene();

	void preSceneRender(JEngine::Engine &);
	void postSceneRender(JEngine::Engine &);
};

