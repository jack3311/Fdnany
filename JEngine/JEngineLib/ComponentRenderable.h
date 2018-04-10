#pragma once

#include "Shader.h"
#include "Renderer.h"

namespace JEngine
{
	class ComponentRenderable
	{
	public:
		int entity;
		Shader * shader;
		Renderer<VertexFormatStandard, true> * renderer;

		ComponentRenderable(const int _entity, Shader * _shader, Renderer<VertexFormatStandard, true> * _renderer);
		~ComponentRenderable();
	};
}
