#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <time.h>
#include <sstream>

#include "Logger.h"

namespace JEngine
{
	Logger * Logger::logger = nullptr;

	Logger::Logger()
	{
		fileStream = std::ofstream(LOG_FILE_PATH);
	}

	Logger::~Logger()
	{
		fileStream.close();
	}

	Logger & Logger::getLogger()
	{
		if (logger == nullptr)
		{
			logger = new Logger();
		}

		return *logger;
	}

	void Logger::log(const std::string & _a)
	{
		auto timestamp = std::time(nullptr);

		tm timeStruct;
		localtime_s(&timeStruct, &timestamp);

		std::stringstream ss;
		ss << std::put_time(&timeStruct, "%d-%m-%Y %H-%M-%S");

		std::cout << ss.str() << ": \t" << _a << std::endl;
		fileStream << ss.str() << ": \t" << _a << std::endl;
	}
}