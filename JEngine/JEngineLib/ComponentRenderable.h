#pragma once

#include "Material.h"

namespace JEngine
{
	class RendererInterface;

	class ComponentRenderable
	{
	public:
		int entity;
		MaterialInterface * material;
		RendererInterface * renderer;

		ComponentRenderable(const int _entity, MaterialInterface * _material, RendererInterface * _renderer);
		~ComponentRenderable();
	};
}
