#include "SystemTest.h"

#include <JEngineLib\Logger.h>
#include <JEngineLib\Util.h>



SystemTest::SystemTest()
{
}


SystemTest::~SystemTest()
{
}

void SystemTest::preRender(JEngine::ECS::Entity & _entity)
{
	JEngine::Logger::get().log(JEngine::strJoinConvert("Pre-render for object with id: ", _entity.getEntityID()));
}

void SystemTest::postRender(JEngine::ECS::Entity & _entity)
{
	JEngine::Logger::get().log(JEngine::strJoinConvert("Post-render for object with id: ", _entity.getEntityID()));
}
