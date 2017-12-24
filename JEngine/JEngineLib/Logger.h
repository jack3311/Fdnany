#pragma once

#include <string>
#include <fstream>

namespace JEngine
{
	#define LOG_FILE_PATH "log.txt" //TODO: configurable

	class Logger
	{
	private:
		Logger();
		~Logger();

		static Logger * logger;

		std::ofstream fileStream;

	public:

		static Logger & getLogger();

		/// 
		/// log:
		/// Logs a given string to the standard output, and the log file
		/// _a - the string to log
		///
		void log(const std::string & _a);
	};
}