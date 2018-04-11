#pragma once

#include <GL\glew.h>
#include <memory>
#include <cassert>

namespace JEngine
{
	template <typename UniformBufferFormat>
	class UniformBufferManager
	{
	private:
		GLuint UBO;
		GLuint uniformBufferLocation

	public:
		UniformBufferFormat bufferData;

		UniformBufferManager();
		~UniformBufferManager();

		bool initialise(GLuint _uniformBufferBindingLocation);

		void flushBufferUpdates();
	};

	template<typename UniformBufferFormat>
	inline UniformBufferManager<UniformBufferFormat>::UniformBufferManager()
	{
	}
	template<typename UniformBufferFormat>
	inline UniformBufferManager<UniformBufferFormat>::~UniformBufferManager()
	{
		assert(Engine::get().isCurrentThreadMain());

		glDeleteBuffers(1, &UBO);
	}

	template<typename UniformBufferFormat>
	inline bool UniformBufferManager<UniformBufferFormat>::initialise(GLuint _uniformBufferBindingLocation)
	{
		assert(Engine::get().isCurrentThreadMain());

		uniformBufferLocation = _uniformBufferBindingLocation;

		memset(&bufferData, 0, sizeof(UniformBufferFormat));
		
		//Setup UBO
		glGenBuffers(1, &UBO);
		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(UniformBufferFormat), nullptr, GL_STATIC_DRAW);
		
		//Bind UBO to binding location
		glBindBufferBase(GL_UNIFORM_BUFFER, _uniformBufferBindingLocation, UBO);
		
		return true;
	}

	template<typename UniformBufferFormat>
	inline void UniformBufferManager<UniformBufferFormat>::flushBufferUpdates()
	{
		assert(Engine::get().isCurrentThreadMain());

		//Copy local uniforms to GPU buffer memory
		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(UniformBufferFormat), &bufferData, GL_STATIC_DRAW);
	}
}