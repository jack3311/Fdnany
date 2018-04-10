#include "ComponentRenderable.h"

JEngine::ComponentRenderable::ComponentRenderable(const int _entity, Shader * _shader, Renderer<VertexFormatStandard, true> * _renderer) :
	entity(_entity),
	shader(_shader),
	renderer(_renderer)
{
	Engine::get().getWorld().addToRenderMatrix(this, entity);
}

JEngine::ComponentRenderable::~ComponentRenderable()
{
	Engine::get().getWorld().removeFromRenderMatrix(this, entity);
}
