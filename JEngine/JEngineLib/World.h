#pragma once

#include "JObject.h"

namespace JEngine
{
	class World : private JObject
	{
	public:
		using JObject::addChild;
		using JObject::removeChild;
		using JObject::getChildren;

	private:


	public:
		World();
		~World();

		bool initialise();

		void update();
		void render() const;
	};
}
