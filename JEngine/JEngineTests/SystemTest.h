#pragma once
#include <JEngineLib\Entity.h>
#include <JEngineLib\System.h>

class SystemTest : public JEngine::ECS::System
{
public:
	SystemTest();
	~SystemTest();

	void preRender(JEngine::ECS::Entity & _entity);

	void postRender(JEngine::ECS::Entity & _entity);
};

