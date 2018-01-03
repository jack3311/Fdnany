#include "ResourceManagement.h"

#include <cassert>
#include <fstream>

#include "Util.h"
#include "Logger.h"
#include "Engine.h"

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

		return true;
	}

	void ResourceManager::cleanUp()
	{
		FreeImage_DeInitialise();
	}

	std::string ResourceManager::constructFullPath(const std::string & _relToInstall) const
	{
		return strJoin({ installPath, _relToInstall });
	}

	void ResourceManager::beginResourceCaching()
	{
		cachingResources = true;
	}

	void ResourceManager::endResourceCaching()
	{
		cachingResources = false;

		//Clean up current cache
		resourcesCache.clear();
	}

	bool ResourceManager::loadResourceTexture(std::shared_ptr<ResourceTexture> & _resource, const std::string & _name)
	{
		//Check cache
		if (cachingResources && checkCache(_resource, _name))
		{
			return true;
		}

		//Load from disk

#ifdef _DEBUG
		{
			std::ifstream filecheck(_name);
			auto ok = filecheck.good();
			filecheck.close();

			if (!ok)
			{
				Logger::getLogger().log(strJoin({ "Image not found: ", _name }), JEngine::LogLevel::WARNING);
				return false;
			}
		}
#endif

		FREE_IMAGE_FORMAT fformat = FreeImage_GetFileType(_name.c_str());
		FIBITMAP * image = FreeImage_Load(fformat, _name.c_str());

		if (image == nullptr)
		{
			Logger::getLogger().log(strJoin({ "Could not load image: ", _name }), JEngine::LogLevel::WARNING);
			return false;
		}


		FREE_IMAGE_COLOR_TYPE colourType = FreeImage_GetColorType(image);

		ResourceTexture::ResourceTextureFormat format = ResourceTexture::ResourceTextureFormat::NONE;

		if (colourType == FREE_IMAGE_COLOR_TYPE::FIC_RGB)
		{
			format = ResourceTexture::ResourceTextureFormat::RGB;
		}
		else if (colourType == FREE_IMAGE_COLOR_TYPE::FIC_RGBALPHA)
		{
			format = ResourceTexture::ResourceTextureFormat::RGBA;
		}
		else
		{
			Logger::getLogger().log(strJoinConvert("Unrecognised colour type: ", colourType), JEngine::LogLevel::WARNING);
			return false;
		}

		unsigned int width = FreeImage_GetWidth(image),
			height = FreeImage_GetHeight(image);

		_resource = std::make_shared<ResourceTexture>(image->data, width, height, format, image);

		//Add to cache
		if (cachingResources)
		{
			resourcesCache[_name] = _resource;
		}

		return true;
	}

	JobLoadResourceTexture::JobLoadResourceTexture(const std::string & _filename) : 
		filename(_filename), loadSuccessful(false)
	{

	}

	void JobLoadResourceTexture::execute()
	{
		

		//Image loading to GPU is delayed to main thread
		
		loadSuccessful = true;
	}

	bool ResourceTexture::initialise()
	{
		assert(Engine::getEngine().isCurrentThreadMain());

		//Load to GPU
		glGenTextures(1, &glTextureID);
		glBindTexture(GL_TEXTURE_2D, glTextureID);
		glTexImage2D(glTextureID, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (fBitmap != nullptr)
		{
			FreeImage_Unload(fBitmap); //Clears data too
			data = nullptr;
		}

		return true;
	}

	ResourceTexture::ResourceTexture(void * _data, unsigned int _width, unsigned int _height, ResourceTextureFormat _format, FIBITMAP * _fBitmap) :
		data(_data), width(_width), height(_height), format(_format), fBitmap(_fBitmap)
	{
	}

	ResourceTexture::~ResourceTexture()
	{
		glDeleteTextures(1, &glTextureID);
	}
}