#pragma once

#include <JEngineLib\JobManagement.h>

class TestJob :
	public JEngine::Job
{
private:

public:
	int num = -1;
	static int numComplete;

	enum {
		NOT_PRIME, PRIME, NOT_YET_CALCULATED
	} isThisNumberPrimeData = NOT_YET_CALCULATED;


	TestJob(int);
	virtual ~TestJob();

	virtual void execute();
};

