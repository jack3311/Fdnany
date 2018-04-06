#pragma once

namespace JEngine
{
	namespace ECS
	{
		class Entity;


		class System
		{
		public:
			virtual void initialise(JEngine::ECS::Entity & _entity) = 0;
			virtual void preRender(JEngine::ECS::Entity & _entity) = 0;
			virtual void postRender(JEngine::ECS::Entity & _entity) = 0;
		};

	}
}