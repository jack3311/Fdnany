#pragma once

#include <memory>

#include "Renderer.h"
#include "Maths.h"

namespace JEngine
{
	class Shader;
	class JObject;

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
		void drawAxes(const vec3 & _pos = { 0.f, 0.f, 0.f }, const vec3 & _u = { 1.f, 0.f, 0.f }, 
			const vec3 & _v = { 0.f, 1.f, 0.f }, const vec3 & _w = { 0.f, 0.f, 1.f }, const float & _size = 1.f);
		void drawTransform(const JObject & _transform);

		void flush();
	};
}