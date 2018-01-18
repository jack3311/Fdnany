#include "DebugRendering.h"

#include "Util.h"
#include "Logger.h"
#include "Shader.h"
#include "ResourceManagement.h"
#include "Engine.h"

namespace JEngine
{
#define MAX_TOTAL_LINE_VERTICES 50
#define MAX_UNIQUE_LINES (MAX_TOTAL_LINE_VERTICES / 2)

	VertexFormatDebugRenderingStandard::VertexFormatDebugRenderingStandard(const vec3 & _position, const vec3 & _color) :
		position(_position), color(_color)
	{
	}

	void VertexFormatDebugRenderingStandard::setupVertexAttributes()
	{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormatDebugRenderingStandard), (GLvoid *)offsetof(VertexFormatDebugRenderingStandard, VertexFormatDebugRenderingStandard::position));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormatDebugRenderingStandard), (GLvoid *)offsetof(VertexFormatDebugRenderingStandard, VertexFormatDebugRenderingStandard::color));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
	}


	DebugRendering * DebugRendering::debugRendering = nullptr;

	DebugRendering::DebugRendering()
	{
		lineRanges.reserve(MAX_UNIQUE_LINES);
		debugShader = std::make_unique<Shader>();
		lineRenderer = std::make_unique<Renderer<VertexFormatDebugRenderingStandard, false>>(GL_LINE_STRIP, false);
	}

	DebugRendering::~DebugRendering()
	{
	}

	DebugRendering & DebugRendering::get()
	{
		return *debugRendering;
	}

	DebugRendering & DebugRendering::create()
	{
		debugRendering = new DebugRendering();

		return *debugRendering;
	}

	bool DebugRendering::initialise()
	{
		auto & resourceManager = JEngine::ResourceManager::getResourceManager();

		//auto debugShaderLoadJob = debugShader->loadFromDiskAsync({
		//	{ Shader::ShaderComponent::VERTEX, resourceManager.constructFullPath("Assets\\debugShader.vert") },
		//	{ Shader::ShaderComponent::FRAGMENT, resourceManager.constructFullPath("Assets\\debugShader.frag") },
		//});
		auto debugShaderLoadJob = debugShader->loadFromDiskAsync({
			{ Shader::ShaderComponent::VERTEX, "Assets\\debugShader.vert" },
			{ Shader::ShaderComponent::FRAGMENT, "Assets\\debugShader.frag" },
		});
		
		ERR_IF(!lineRenderer->initialise({}, MAX_TOTAL_LINE_VERTICES), "Could not initialise standard debug renderer");

		debugShaderLoadJob->waitUntilFinished();
		ERR_IF(!debugShaderLoadJob->wasSuccessful(), "Could not load debug shader");
		ERR_IF(!debugShader->initialise(), "Could not initialise debug shader");

		return true;
	}

	void DebugRendering::drawLine(const std::vector<VertexFormatDebugRenderingStandard> & _vertices)
	{
		assert(_vertices.size() >= 2);

		auto & lineRendererVertices = lineRenderer->getVertices();

		//Set range start point
		BufferRange range;
		auto size1 = static_cast<unsigned int>(lineRendererVertices.size());
		if (size1 == 0)
		{
			range.start = 0;
		}
		else
		{
			range.start = size1 - 1u;
		}

		//Add new vertices to the end
		lineRendererVertices.insert(lineRendererVertices.begin(), _vertices.begin(), _vertices.end());

		//Set range end point
		range.end = static_cast<unsigned int>(lineRendererVertices.size()) - 1u;

		//Add range to ranges
		lineRanges.emplace_back(range);
	}

	void DebugRendering::flush()
	{
		assert(Engine::get().isCurrentThreadMain());

		auto & engine = Engine::get();

		debugShader->begin(engine.getStandardView());
		{
			//Flush line renderer
			lineRenderer->updateData();

			//Draw each line range
			for (const auto & range : lineRanges)
			{
				lineRenderer->draw(mat4(), range);
			}
		}
		debugShader->end();

		//Clear lines ranges and vertices
		lineRanges.clear();
		lineRenderer->getVertices().clear();
	}
}