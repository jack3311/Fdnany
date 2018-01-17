#include "DebugRendering.h"

namespace JEngine
{
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

	DebugRendering::DebugRendering() :
		standardRenderer({})
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

	void DebugRendering::drawLine(std::initializer_list<const vec3 &> _vertices)
	{

	}
}