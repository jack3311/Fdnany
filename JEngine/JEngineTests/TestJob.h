#pragma once

#include <JEngineLib\JobManagement.h>

class TestJob :
	public JEngine::Job
{
private:

public:
	int num = -1;

	TestJob(int);
	virtual ~TestJob();

	virtual void execute();
};

