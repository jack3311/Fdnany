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
	}

	Logger::~Logger()
	{
		if (fileStream.is_open())
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

	void Logger::log(const std::string & _a, LogLevel _logLevel)
	{
		auto timestamp = std::time(nullptr);

		tm timeStruct;
		localtime_s(&timeStruct, &timestamp);

		std::stringstream ss;
		ss << std::put_time(&timeStruct, "%d-%m-%Y %H-%M-%S:\t");

		switch (_logLevel)
		{
		case LogLevel::INFO:
			ss << "INFO:\t";
			break;
		case LogLevel::WARNING:
			ss << "WARN:\t";
			break;
		case LogLevel::ERROR:
			ss << "ERR:\t";
			break;
		default:
			ss << "OTH:\t";
			break;
		}

		std::cout << ss.str() << _a << std::endl;

		if (fileStream.is_open())
			fileStream << ss.str() << _a << std::endl;
	}
	bool Logger::initialise(const std::string & _filePath)
	{
		fileStream = std::ofstream(_filePath);

		return fileStream.is_open();
	}
}