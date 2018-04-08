#pragma once

#include "ECSConstants.h"

namespace JEngine
{
	namespace ECS
	{
		class Entity;

		class System
		{
		public:
			//virtual void initialise(Entity & _entity);
			virtual void preRender(Entity & _entity);
			virtual void postRender(Entity & _entity);

			static constexpr unsigned int getExecutionPriority()
			{
				return SYSTEM_PRIORITY_DEFAULT;
			}
		};

	}
}