#pragma once

#include <string>
#include <fstream>
#include <mutex>

namespace JEngine
{
	enum class LogLevel : unsigned __int8
	{
		INFO,
		WARNING,
		ERROR
	};
	
	class Logger
	{
	private:
		static Logger * logger;

		Logger();
		Logger(const Logger &) = delete;
		~Logger();
	public:
		static Logger & getLogger();
		static Logger & create();


	private:
		std::ofstream fileStream;
		std::mutex loggingMutex;

	public:
		/// 
		/// log:
		/// Logs a given string to the standard output, and the log file
		/// _a - the string to log
		///
		void log(const std::string &, LogLevel = LogLevel::INFO);

		bool initialise(const std::string & _filePath);
		void cleanUp();
	};
}