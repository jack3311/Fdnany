#include "DebugRendering.h"

namespace JEngine
{
	DebugRendering * DebugRendering::debugRendering = nullptr;

	DebugRendering::DebugRendering()
	{
	}

	DebugRendering::~DebugRendering()
	{
	}

	DebugRendering & DebugRendering::get()
	{
		return *debugRendering;
	}

	void DebugRendering::create()
	{
		debugRendering = new DebugRendering();
	}

	bool DebugRendering::initialise()
	{


		return true;
	}
}