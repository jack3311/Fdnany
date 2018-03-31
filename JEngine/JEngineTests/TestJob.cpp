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
	JEngine::Logger::get().log(JEngine::strJoinConvert("Starting job for: ", num));

	for (int j = 0; j < num; ++j)
	{
		bool isPrime = true;
		for (int i = 2; i * i <= num; ++i)
		{
			if (num % i == 0) {
				isPrime = false;
				break;
			}
		}

		JEngine::Logger::get().log(JEngine::strJoinConvert(j, " prime: ", isPrime));
	}
}
