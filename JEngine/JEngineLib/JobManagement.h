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
		bool isFinished;
		bool spawnNewThread;

		std::condition_variable isFinishedCV;
		std::mutex isFinishedCVMutex;

		JEvent<const Job *> jEvent;

	protected:
		bool successful;

	public:
		Job(/*bool _spawnNewThread = false*/);

		virtual void execute() = 0;
		void setComplete();

		void waitUntilFinished();

		JEvent<const Job *> & getEvent();

		bool wasSuccessful() const;

		/*bool getSpawnNewThread() const;*/
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

	class JobCallFunction : public Job
	{
	private:
		std::function<bool()> function;

	public:
		JobCallFunction(std::function<bool()>);

		virtual void execute();
	};

	class JobAggregate : public Job
	{
	private:
		std::mutex jobsMutex;
		std::vector<std::shared_ptr<Job>> jobs;

	public:
		JobAggregate();

		virtual void execute();

		void addJob(std::shared_ptr<Job> _job);

		//Override
		void waitUntilFinished();

		void waitUntilAllSubJobsFinishedOrShutdown();
	};
}