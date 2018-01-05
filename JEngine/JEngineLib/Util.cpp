#include "Util.h"

#include <Windows.h>
#include <stdio.h>
#include <thread>
#include <fstream>


bool readAllFile(std::string & _out, const std::string & _path)
{
	std::ifstream file(_path, std::ios::in);

	if (!file.good())
	{
		return false;
	}

	file.seekg(0, std::ios::end);
	_out.resize((unsigned int)file.tellg());
	file.seekg(0, std::ios::beg);
	file.read(&_out[0], _out.size());
	file.close();

	return true;
}

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