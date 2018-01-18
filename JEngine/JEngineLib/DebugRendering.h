#pragma once

#include <memory>

#include "Renderer.h"

namespace JEngine
{
	class Shader;

	struct VertexFormatDebugRenderingStandard
	{
		vec3 position;
		vec3 color;

		VertexFormatDebugRenderingStandard(const vec3 &, const vec3 &);

		static void setupVertexAttributes();
	};

	class DebugRendering
	{
	private:
		static DebugRendering * debugRendering;

	public:
		DebugRendering();
		~DebugRendering();
		DebugRendering(const DebugRendering &) = delete;
		const DebugRendering & operator=(const DebugRendering &) = delete;

		static DebugRendering & get();
		static DebugRendering & create();

	private:
		std::unique_ptr<Renderer<VertexFormatDebugRenderingStandard, false>> lineRenderer;
		std::vector<BufferRange> lineRanges;

		std::unique_ptr<Shader> debugShader;

	public:
		bool initialise();

		void drawLine(const std::vector<VertexFormatDebugRenderingStandard> & _vertices);

		void flush();
	};
}