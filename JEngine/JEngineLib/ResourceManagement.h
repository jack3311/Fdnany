#pragma once

#include <vector>
#include <string>

#include "JobManagement.h"

namespace JEngine
{
	class Resource
	{
	public:
		Resource();
		Resource(const Resource &) = delete;
		~Resource();
	};

	class ResourceTexture : public Resource
	{
	private:
		unsigned int glTextureID;

	};

	class JobLoadResourceTexture : public Job
	{
	private:
		std::string filename;

	public:
		bool loadSuccessful;

		JobLoadResourceTexture(const std::string &);

		virtual void execute();
	};





	class ResourcePackage
	{
	public:
		std::vector<Resource> resources;
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

	public:
		ResourceManager();
		~ResourceManager();

		bool initialise();
		void cleanUp();

		std::string constructFullPath(const std::string & _relToInstall);
	};
}