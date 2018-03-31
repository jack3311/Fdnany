#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <time.h>
#include <sstream>
#include <cassert>

#include "Logger.h"

namespace JEngine
{
	Logger * Logger::logger = nullptr;

	Logger::Logger()
	{
	}

	Logger::~Logger()
	{
	}

	Logger & Logger::get()
	{
		return *logger;
	}

	Logger & Logger::create()
	{
		assert(logger == nullptr);
		logger = new Logger();
		return *logger;
	}

	void Logger::log(const std::string & _a, LogLevel _logLevel)
	{
#ifndef _DEBUG
		if (_logLevel == LogLevel::INFO)
		{
			//return;
		}
#endif

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

		ss << _a << std::endl;

		std::string str = ss.str();
		const char * c_str = str.c_str();

		loggingMutex.lock();
		{
			printf(ss.str().c_str());

			if (fileStream.is_open())
			{
				fileStream.write(ss.str().c_str(), ss.str().length());
			}
		}
		loggingMutex.unlock();
	}

	bool Logger::initialise(const std::string & _filePath)
	{
		fileStream = std::ofstream(_filePath, std::ios_base::out);

		return fileStream.is_open();
	}

	void Logger::cleanUp()
	{
		if (fileStream.is_open())
		{
			fileStream.flush();
			fileStream.close();
		}
	}
}