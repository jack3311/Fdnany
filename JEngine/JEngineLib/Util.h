#include <string>
#include <sstream>

#define ERR_IF(exp, msg) { if (exp) { Logger::getLogger().log(msg, LogLevel::ERROR); return false; } }

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