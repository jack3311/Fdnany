#pragma once

#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

namespace JEngine
{
	class ECS::Entity;
	class ComponentRenderable;
	class RendererInterface;
	class MaterialInterface;

	class World
	{
	private:
		ECS::EntityManager entityManager;
		ECS::ComponentManager componentManager;
		ECS::SystemManager systemManager;

		//Sort by material and renderer components
		std::map<MaterialInterface *, std::multimap<RendererInterface *, ECS::Entity *>> renderMatrix;

		void drawRenderMatrix() const;

		void updateEntityMatrices();

	public:
		World();
		~World();

		bool initialise();

		void update();
		void render() const;

		ECS::EntityManager & getEntityManager();
		ECS::ComponentManager & getComponentManager();
		ECS::SystemManager & getSystemManager();

		void preRender();
		void postRender();

		void addToRenderMatrix(ComponentRenderable * _renderable, const int _entity);
		void removeFromRenderMatrix(ComponentRenderable * _renderable, const int _entity);
	};
}
