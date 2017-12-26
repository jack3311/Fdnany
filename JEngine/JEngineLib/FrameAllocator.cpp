#include "FrameAllocator.h"

#include <malloc.h>

namespace JEngine
{
	FrameAllocator::FrameAllocator() :
		memStart(nullptr),
		currentAddress(nullptr)
	{
	}

	FrameAllocator::~FrameAllocator()
	{
	}

	bool FrameAllocator::initialise(size_t _maxSize)
	{
		maxSize = _maxSize;
		
		//Allocate once
		memStart = reinterpret_cast<char *>(malloc(_maxSize));
		currentAddress = memStart;
		memEnd = memStart + _maxSize;

		return true;
	}

	void FrameAllocator::cleanUp()
	{
		free(memStart);
	}

	void FrameAllocator::reset()
	{
		currentAddress = memStart;
	}

}
