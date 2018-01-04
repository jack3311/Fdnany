#include "Util.h"

#include <Windows.h>
#include <stdio.h>
#include <thread>


std::vector<std::string> getFilesInDirectory(const std::string & _directory)
{
	std::vector<std::string> files;

	WIN32_FIND_DATAA findFileData;
	HANDLE hFind;

	hFind = FindFirstFileA(_directory.c_str(), &findFileData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				files.push_back(findFileData.cFileName);
			}
		} while (FindNextFileA(hFind, &findFileData));

		FindClose(hFind);
	}

	return files;
}

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