#pragma once

#include <string>
#include <fstream>

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


	private:
		std::ofstream fileStream;

	public:
		/// 
		/// log:
		/// Logs a given string to the standard output, and the log file
		/// _a - the string to log
		///
		void log(const std::string &, LogLevel = LogLevel::INFO);

		bool initialise(const std::string & _filePath);
	};
}