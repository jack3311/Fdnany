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
			return;
		}
	}

	isThisNumberPrimeData = PRIME;
}
