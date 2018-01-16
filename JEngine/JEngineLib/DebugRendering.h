#pragma once

#include "Renderer.h"

namespace JEngine
{
	struct VertexFormatDebugRenderingStandard
	{

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
		Renderer<

	public:
		bool initialise();

	};
}