#include "Util.h"

#include <Windows.h>
#include <stdio.h>
#include <thread>


std::string getWorkingDirectory()
{
	char buff[FILENAME_MAX];
	GetCurrentDirectoryA(FILENAME_MAX, buff);
	return std::string(buff);
}

std::string strJoin(std::initializer_list<std::string> _elements)
{
	std::string result;
	for (std::string s : _elements)
		result += s;
	return result;
}