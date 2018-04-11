#pragma once

#include "UniformBuffer.h"
#include "UniformBufferFormats.h"

namespace JEngine
{
	class UniformBuffersManager
	{
	private:
		std::unique_ptr<UniformBuffer<UniformBufferFormatViewInfo>> uniformBufferViewInfo;

	public:
		UniformBuffersManager();
		~UniformBuffersManager();

		bool initialise();

		void updateUniformBuffers();
	};
}