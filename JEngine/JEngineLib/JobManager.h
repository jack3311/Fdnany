#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <mutex>
#include <thread>

namespace JEngine
{
	class Worker
	{
	private:
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

	class Job
	{
	private:
		void start();
		void waitUntilFinished();

	public:
		virtual void doWork() = 0;
	};

	class JobManager
	{
	private:
		unsigned int numWorkers;
		std::vector<std::unique_ptr<Worker>> workers;
		std::queue<std::shared_ptr<Job>> jobs;
		std::mutex jobsMutex;

	public:
		JobManager();
		JobManager(const JobManager &) = delete;
		~JobManager();

		bool initialise();

		void start();
		void stop();

		void pushJob(std::shared_ptr<Job> job);
		std::shared_ptr<Job> popJob();
	};
}