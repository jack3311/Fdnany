#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <mutex>
#include <thread>

#include "JEvent.h"

namespace JEngine
{
	class JobManager;

	class Job
	{
	private:
		bool isFinished = false;

		std::condition_variable isFinishedCV;
		std::mutex isFinishedCVMutex;

		JEventParameterised<const Job *> jEvent;

	public:
		virtual void execute() = 0;
		void setComplete();

		void waitUntilFinished();

		JEventParameterised<const Job *> & getEvent();
	};

	class Worker
	{
	private:
		bool shouldBeWorking;
		bool isWorking;
		std::shared_ptr<Job> currentJob;
		std::thread workThread;
		std::shared_ptr<JobManager> parentJobManager;

	public:
		Worker(const std::shared_ptr<JobManager> &);
		Worker(const Worker &) = delete;
		~Worker();

		void start();
		void working();
		void stop();
	};

	class JobManager
	{
	private:
		unsigned int numWorkers;
		std::vector<std::unique_ptr<Worker>> workers;
		std::queue<std::shared_ptr<Job>> jobs;
		bool _hasJobsOrShutdown;
		std::condition_variable hasJobsCV;
		std::mutex hasJobsCVMutex;


		void start();

	public:
		JobManager();
		JobManager(const JobManager &) = delete;
		~JobManager();

		bool initialise();

		void stop();

		void enqueueJob(std::shared_ptr<Job> job);
		bool dequeueJob(std::shared_ptr<Job> & job);

		void waitForJobOrShutdown();

		bool hasJobs() const;

		unsigned int getJobCount() const;
		unsigned int getNumWorkers() const;
	};
}