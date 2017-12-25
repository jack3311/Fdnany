#include "TestJob.h"

#include <JEngineLib\Logger.h>
#include <JEngineLib\Util.h>

TestJob::TestJob(int _num) : num(_num)
{
}


TestJob::~TestJob()
{
}

void TestJob::execute()
{
	for (int i = 2; i * i <= num; ++i)
	{
		if (num % i == 0) {
			isThisNumberPrimeData = NOT_PRIME;
			//JEngine::Logger::getLogger().log(strJoinConvert(num, " result: ", isThisNumberPrimeData, " from thread: ", std::this_thread::get_id()));
			return;
		}
	}

	isThisNumberPrimeData = PRIME;
	

	//JEngine::Logger::getLogger().log(strJoinConvert(num, " result: ", isThisNumberPrimeData, " from thread: ", std::this_thread::get_id()));
}
