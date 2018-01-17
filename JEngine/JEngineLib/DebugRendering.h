#pragma once

#include "Renderer.h"

namespace JEngine
{
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
		static void create();

	private:
		Renderer<VertexFormatDebugRenderingStandard, false> standardRenderer;

	public:
		bool initialise();

		void drawLine(std::initializer_list<const vec3 &> _vertices);

	};
}