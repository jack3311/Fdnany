#include "ResourceManagement.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <FreeImagePlus.h>

#include <cassert>
#include <fstream>

#include "Util.h"

namespace JEngine
{
	ResourceManager * ResourceManager::resourceManager = nullptr;
	
	ResourceManager & ResourceManager::getResourceManager()
	{
		return *resourceManager;
	}

	ResourceManager & ResourceManager::create()
	{
		assert(resourceManager == nullptr);
		resourceManager = new ResourceManager();
		return *resourceManager;
	}

	ResourceManager::ResourceManager()
	{
	}


	ResourceManager::~ResourceManager()
	{
	}

	bool ResourceManager::initialise()
	{
		FreeImage_Initialise();

		installPath = getWorkingDirectory() + "\\";

		return false;
	}

	void ResourceManager::cleanUp()
	{
		FreeImage_DeInitialise();
	}

	std::string ResourceManager::constructFullPath(const std::string & _relToInstall)
	{
		return strJoin({ installPath, _relToInstall });
	} 

	JobLoadResourceTexture::JobLoadResourceTexture(const std::string & _filename) : filename(_filename)
	{

	}

	void JobLoadResourceTexture::execute()
	{
		std::ifstream filecheck(filename);
		auto ok = filecheck.good();

		FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename.c_str());
		FIBITMAP * image = FreeImage_Load(format, filename.c_str());
		FREE_IMAGE_COLOR_TYPE colourType = FreeImage_GetColorType(image);
	}
}