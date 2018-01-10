#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <FreeImage.h>

#include <vector>
#include <string>
#include <unordered_map>
#include <cassert>
#include <mutex>

#include "JobManagement.h"

namespace JEngine
{
	class Resource
	{
	public:
		virtual ~Resource();
	};

	class ResourceTexture : public Resource
	{
	private:
		FIBITMAP * fBitmap;
		void * data;

#ifdef _DEBUG
		bool initialised;
#endif

	public:
		enum ResourceTextureFormat 
		{
			NONE = GL_NONE,
			RGB = GL_RGB,
			RGBA = GL_RGBA,
			R = GL_RED
		} format;

		unsigned int glTextureID;
		unsigned int width, height;

		ResourceTexture(void * _data, unsigned int _width, unsigned int _height, ResourceTextureFormat _format, FIBITMAP * _fBitmap = nullptr);
		virtual ~ResourceTexture();

		bool initialise();

		void bind(GLenum _unit) const;
	};

	class JobLoadResourceTexture : public Job
	{
	private:
		std::string name;

	public:
		bool loadSuccessful; //TODO: change to parent
		std::shared_ptr<ResourceTexture> texture;

		JobLoadResourceTexture(const std::string &);

		virtual void execute();
	};


	class ResourceManager
	{
	private:
		static ResourceManager * resourceManager;
	public:
		static ResourceManager & getResourceManager();
		static ResourceManager & create();


	private:
		std::string installPath;
		bool cachingResources;

		std::mutex resourcesCacheMutex;
		std::unordered_map<std::string, std::shared_ptr<Resource>> resourcesCache;

		
		template <typename T>
		bool checkCache(std::shared_ptr<T> & _resource, const std::string & _name);

	public:
		ResourceManager();
		ResourceManager(const ResourceManager &) = delete;
		~ResourceManager();
		ResourceManager & operator=(const ResourceManager &) = delete;

		bool initialise();
		void cleanUp();


		void beginResourceCaching();
		void endResourceCaching();

		bool loadResourceTexture(std::shared_ptr<ResourceTexture> & _resource, const std::string & _name);
		void loadResourceTextureAsync(std::shared_ptr<JobLoadResourceTexture> & _job, const std::string & _name);
	
		std::string constructFullPath(const std::string & _relToInstall) const;
	};

	template <typename T>
	inline bool ResourceManager::checkCache(std::shared_ptr<T> & _resource, const std::string & _name)
	{
		std::lock_guard<std::mutex> guard(resourcesCacheMutex);

		auto itr = resourcesCache.find(_name);
		if (itr != resourcesCache.end())
		{
			//Resource is cached

			auto resourcePtr = itr->second;

			Logger::getLogger().log(strJoin({ "Found cached resource: ", _name }));

			_resource = std::dynamic_pointer_cast<T>(resourcePtr);

			//Verify resource type
			if (!_resource)
			{
				Logger::getLogger().log(strJoin({ "Cached resource type for resource: '", _name, "' does not match expected" }), LogLevel::WARNING);
				return false;
			}

			return true;
		}

		return false;
	}
}