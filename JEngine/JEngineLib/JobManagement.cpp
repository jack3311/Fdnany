#include "JobManagement.h"

#include <cassert>

namespace JEngine
{
	Worker::Worker(const std::shared_ptr<JobManager> & _parentJobManager) :
		parentJobManager(_parentJobManager),
		shouldBeWorking(false),
		isWorking(false)
	{
	}

	Worker::~Worker()
	{
	}

	void Worker::start()
	{
		shouldBeWorking = true;

		workThread = std::thread(&Worker::working, this);
		workThread.detach();
	}

	void Worker::working()
	{
		isWorking = true;

		while (shouldBeWorking)
		{
			//Ensure there is some work to do
			parentJobManager->waitForJobOrShutdown();

			//Check we should still be working (should not shutdown)
			if (!shouldBeWorking) break;

			//Get the job
			std::shared_ptr<Job> job;
			if (!parentJobManager->dequeueJob(job)) continue; //If there are no jobs, wait again

			//Do the work
			job->execute();

			//Set job as complete
			job->setComplete();
		}

		isWorking = false;
	}

	void Worker::stop()
	{
		shouldBeWorking = false;
	}

	

	void Job::setComplete()
	{
		{
			std::lock_guard<std::mutex> lk(isFinishedCVMutex);
			isFinished = true;
		}

		isFinishedCV.notify_all();

		jEvent.triggerEvent(this);
	}

	void Job::waitUntilFinished()
	{
		std::unique_lock<std::mutex> lk(isFinishedCVMutex);
		isFinishedCV.wait(lk, [this]() { return isFinished; });
		lk.unlock();
	}

	JEvent<const Job *> & Job::getEvent()
	{
		return jEvent;
	}

	bool Job::wasSuccessful() const
	{
		return successful;
	}



	JobManager::JobManager() :
		_hasJobsOrShutdown(false)
	{
	}

	JobManager::~JobManager()
	{
	}

	bool JobManager::initialise()
	{
		//Set up workers
		numWorkers = std::thread::hardware_concurrency() - 1u;

		workers.resize(numWorkers);
		for (unsigned int i = 0; i < numWorkers; ++i)
		{
			workers[i] = std::make_unique<Worker>(std::shared_ptr<JobManager>(this));
		}

		start();

		return true;
	}

	void JobManager::start()
	{
		//Start all of the workers
		for (auto & worker : workers)
		{
			worker->start();
		}
	}

	void JobManager::stop()
	{
		//Set the workers to stop after they have finished their work
		for (auto & worker : workers)
		{
			worker->stop();
		}
	}

	void JobManager::enqueueJob(std::shared_ptr<Job> job)
	{
		{
			std::lock_guard<std::mutex> lk(hasJobsCVMutex);
			jobs.push(job);

			_hasJobsOrShutdown = true;
		}

		hasJobsCV.notify_one();
	}

	bool JobManager::dequeueJob(std::shared_ptr<Job> & job)
	{
		{
			std::lock_guard<std::mutex> lk(hasJobsCVMutex);

			if (jobs.empty()) return false;

			job = jobs.front();
			jobs.pop();

			_hasJobsOrShutdown = !jobs.empty();
		}
		return true;
	}

	void JobManager::waitForJobOrShutdown()
	{
		if (_hasJobsOrShutdown) return;

		std::unique_lock<std::mutex> lk(hasJobsCVMutex);

		hasJobsCV.wait(lk, [this]() { return _hasJobsOrShutdown; });

		lk.unlock();
	}

	bool JobManager::hasJobs() const
	{
		return _hasJobsOrShutdown;
	}

	unsigned int JobManager::getJobCount() const
	{
		return static_cast<unsigned int>(jobs.size());
	}

	unsigned int JobManager::getNumWorkers() const
	{
		return numWorkers;
	}
	JobCallFunction::JobCallFunction(std::function<bool()> _function) :
		function(_function)
	{
	}
	void JobCallFunction::execute()
	{
		successful = function();
	}
}