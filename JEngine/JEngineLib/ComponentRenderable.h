#pragma once

namespace JEngine
{
	class Material;
	class RendererInterface;

	class ComponentRenderable
	{
	public:
		int entity;
		Material * material;
		RendererInterface * renderer;

		ComponentRenderable(const int _entity, Material * _material, RendererInterface * _renderer);
		~ComponentRenderable();
	};
}
