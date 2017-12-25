#include "Util.h"

std::string strJoin(std::initializer_list<std::string> _elements)
{
	std::string result;
	for (std::string s : _elements)
		result += s;
	return result;
}