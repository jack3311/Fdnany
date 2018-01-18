#pragma once

#include <string>
#include <sstream>
#include <vector>

namespace JEngine
{
#define ERR_IF(exp, msg) { if (exp) { Logger::getLogger().log(msg, LogLevel::ERROR); return false; } else {} }

	template <typename T>
	struct Range
	{
		T start, end;

		Range() = default;
		Range(const Range &) = default;

		Range(const T & _start, const T & _end) :
			start(_start), end(_end)
		{
			static_assert(std::is_arithmetic<T>::value, "Range type must be arithmetic");
		}

		T getLength() const
		{
			return end - start;
		}
	};

	using BufferRange = Range<unsigned int>;

	bool readAllFile(std::string & _out, const std::string & _path);
	std::vector<std::string> getFilesInDirectory(const std::string & _directory);

	std::string getWorkingDirectory();

	std::string strJoin(std::initializer_list<std::string> _elements);

	namespace {
		template <typename T>
		void strJoinConvert(std::stringstream & _ss, T & _first)
		{
			_ss << _first;
		}
		template <typename T, typename... Args>
		void strJoinConvert(std::stringstream & _ss, T & _first, Args ..._args)
		{
			_ss << _first;
			strJoinConvert(_ss, _args...);
		}
	}
	template <typename T, typename... Args>
	std::string strJoinConvert(T & _first, Args ..._args)
	{
		std::stringstream ss;
		ss << _first;
		strJoinConvert(ss, _args...);
		return ss.str();
	}

	template <typename T>
	std::string toString(const T & _val)
	{
		std::stringstream ss;
		ss << _val;
		return ss.str();
	}

	template <typename T>
	T max(const T & _a, const T & _b)
	{
		return _a > _b ? _a : _b;
	}

	template <typename T>
	T min(const T & _a, const T & _b)
	{
		return _a < _b ? _a : _b;
	}
}