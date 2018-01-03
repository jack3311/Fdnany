#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <FreeImage.h>

#include <vector>
#include <string>
#include <unordered_map>
#include <cassert>

#include "JobManagement.h"

namespace JEngine
{
	class Resource
	{
	};

	class ResourceTexture : public Resource
	{
	private:
		FIBITMAP * fBitmap;
		void * data;

	public:
		enum ResourceTextureFormat 
		{
			NONE = GL_NONE,
			RGB = GL_RGB,
			RGBA = GL_RGBA
		} format;

		unsigned int glTextureID;
		unsigned int width, height;

		ResourceTexture(void * _data, unsigned int _width, unsigned int _height, ResourceTextureFormat _format, FIBITMAP * _fBitmap = nullptr);
		~ResourceTexture();

		bool initialise();
	};

	class JobLoadResourceTexture : public Job
	{
	private:
		std::string filename;

	public:
		bool loadSuccessful;
		std::shared_ptr<ResourceTexture> texture;

		JobLoadResourceTexture(const std::string &);

		virtual void execute();
	};


	class ResourceManager //FOR BATCHING RESOURCE LOADS INTO ResourcePackages
	{
	private:
		static ResourceManager * resourceManager;
	public:
		static ResourceManager & getResourceManager();
		static ResourceManager & create();


	private:
		std::string installPath;
		bool cachingResources;

		std::unordered_map<std::string, std::shared_ptr<Resource>> resourcesCache;

		std::string constructFullPath(const std::string & _relToInstall) const;

		template <typename T>
		bool checkCache(std::shared_ptr<T> & _resource, const std::string & _name) const;

	public:
		ResourceManager();
		~ResourceManager();

		bool initialise();
		void cleanUp();


		void beginResourceCaching();
		void endResourceCaching();

		bool loadResourceTexture(std::shared_ptr<ResourceTexture> & _resource, const std::string & _name);
		void loadResourceTextureAsync(std::shared_ptr<JobLoadResourceTexture> & _job, const std::string & _name);
	};

	template <typename T>
	inline bool ResourceManager::checkCache(std::shared_ptr<T> & _resource, const std::string & _name) const
	{
		if ((auto itr = resourcesCache.find(_name)) != resourcesCache.end())
		{
			//Resource is cached

			auto resourcePtr = *itr;

			//Verify resource type
			if (typeid(T) != typeid(*resourcePtr))
			{
				Logger::getLogger().log(strJoin({ "Cached resource type for resource: '", _name, "' is '", 
					typeid(*resourcePtr).name(), "', but expected '", typeid(T).name(), "'" }), LogLevel::WARNING);
				return false;
			}

			_resource = resourcePtr;
			return true;
		}

		return false;
	}
}