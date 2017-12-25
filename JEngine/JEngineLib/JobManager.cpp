#include "JobManager.h"

namespace JEngine
{
	Worker::Worker(const std::shared_ptr<JobManager> & _parentJobManager) :
		parentJobManager(_parentJobManager)
	{
	}

	Worker::~Worker()
	{
	}

	void Worker::start()
	{
		isWorking = true;

		workThread = std::thread(this->working);
		workThread.detach();
	}

	void Worker::working()
	{
		while (true)
		{
			//Check if there is some work to do
			{
				std::lock_guard(parentJobManager->)
			}
		}
	}

	void Worker::stop()
	{
		isWorking = false;
	}



	void Job::start()
	{
	}
	
	void Job::waitUntilFinished()
	{
	}



	JobManager::JobManager()
	{
	}

	JobManager::~JobManager()
	{
	}

	bool JobManager::initialise()
	{
		numWorkers = std::thread::hardware_concurrency() - 1u;

		workers.resize(numWorkers);
		for (unsigned int i = 0; i < numWorkers; ++i)
		{
			workers[i] = std::make_unique<Worker>();
		}

		return true;
	}

	void JobManager::start()
	{
	}

	void JobManager::stop()
	{
	}

	void JobManager::pushJob(std::shared_ptr<Job> job)
	{
	}

	std::shared_ptr<Job> JobManager::popJob()
	{
		std::move()
		std::shared_ptr<Job> fr = jobs.top();
		return std::shared_ptr<Job>();
	}

	

}