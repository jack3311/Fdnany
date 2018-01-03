#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <FreeImage.h>

#include <vector>
#include <string>

#include "JobManagement.h"

namespace JEngine
{
	class Resource
	{
	public:
		virtual void cleanUp();
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

		bool initialise();
	
		virtual void cleanUp();
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