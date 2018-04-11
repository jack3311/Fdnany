#pragma once

#include <GL\glew.h>
#include <memory>
#include <cassert>

namespace JEngine
{
	template <typename UniformBufferFormat>
	class UniformBuffer
	{
	private:
		GLuint UBO;
		GLuint uniformBufferBindingLocation;

	public:
		UniformBufferFormat bufferData;

		UniformBuffer();
		~UniformBuffer();

		bool initialise(GLuint _uniformBufferBindingLocation);

		void flushBufferUpdates();

		void bind();
	};

	template<typename UniformBufferFormat>
	inline UniformBuffer<UniformBufferFormat>::UniformBuffer()
	{
	}
	template<typename UniformBufferFormat>
	inline UniformBuffer<UniformBufferFormat>::~UniformBuffer()
	{
		assert(Engine::get().isCurrentThreadMain());

		glDeleteBuffers(1, &UBO);
	}

	template<typename UniformBufferFormat>
	inline bool UniformBuffer<UniformBufferFormat>::initialise(GLuint _uniformBufferBindingLocation)
	{
		assert(Engine::get().isCurrentThreadMain());

		uniformBufferBindingLocation = _uniformBufferBindingLocation;

		//Zero memory
		memset(&bufferData, 0, sizeof(UniformBufferFormat));
		
		//Setup UBO
		glGenBuffers(1, &UBO);
		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(UniformBufferFormat), nullptr, GL_STATIC_DRAW);
		
		//Bind
		bind();
		
		return true;
	}

	template<typename UniformBufferFormat>
	inline void UniformBuffer<UniformBufferFormat>::flushBufferUpdates()
	{
		assert(Engine::get().isCurrentThreadMain());

		//Copy local uniforms to GPU buffer memory
		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(UniformBufferFormat), &bufferData, GL_STATIC_DRAW);
	}

	template<typename UniformBufferFormat>
	inline void UniformBuffer<UniformBufferFormat>::bind()
	{
		assert(Engine::get().isCurrentThreadMain());

		//Bind UBO to binding location
		glBindBufferBase(GL_UNIFORM_BUFFER, uniformBufferBindingLocation, UBO);
	}
}