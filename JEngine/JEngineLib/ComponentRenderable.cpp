#include "ComponentRenderable.h"

#include "Engine.h"
#include "Renderer.h"
#include "Material.h"
#include "World.h"

JEngine::ComponentRenderable::ComponentRenderable(const int _entity, MaterialInterface * _material, RendererInterface * _renderer) :
	entity(_entity),
	material(_material),
	renderer(_renderer)
{
	Engine::get().getWorld().addToRenderMatrix(this, entity);
}

JEngine::ComponentRenderable::~ComponentRenderable()
{
	Engine::get().getWorld().removeFromRenderMatrix(this, entity);
}
