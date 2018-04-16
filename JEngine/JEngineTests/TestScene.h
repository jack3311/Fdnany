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
#include <JEngineLib\UniformBufferFormats.h>
#include <JEngineLib\RendererTerrain.h>

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
	std::shared_ptr<JEngine::Renderer<MyVertexFormat, true>> renderer1;
	std::shared_ptr<JEngine::Renderer<MyVertexFormat, true>> renderer2;


	std::shared_ptr<JEngine::Shader> terrainShader;
	std::shared_ptr<JEngine::RendererTerrain> rendererTerrain;
	std::shared_ptr<JEngine::Material<UniformBufferFormatNull>> materialTerrain;

	std::shared_ptr<JEngine::Shader> textShader;
	std::shared_ptr<JEngine::ResourceFont> testFont;
	std::shared_ptr<JEngine::RendererText> rendererText;

	std::shared_ptr<JEngine::Camera> testCamera;

	std::shared_ptr<JEngine::ResourceTexture> testTexture;

	JEngine::ECS::Entity * entity1;

	std::shared_ptr<JEngine::Material<UniformBufferFormatNull>> material1;
	std::shared_ptr<JEngine::Material<UniformBufferFormatNull>> material2;

public:
	TestScene();
	~TestScene();

	void preSceneRender(JEngine::Engine &);
	void postSceneRender(JEngine::Engine &);
};

