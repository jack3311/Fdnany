#include "StackAllocator.h"

#include <malloc.h>

namespace JEngine
{
	StackAllocator::StackAllocator() :
		memStart(nullptr),
		currentAddress(nullptr)
	{
	}

	StackAllocator::~StackAllocator()
	{
	}

	bool StackAllocator::initialise(size_t _maxSize)
	{
		maxSize = _maxSize;
		
		//Allocate once
		memStart = reinterpret_cast<char *>(malloc(_maxSize));
		currentAddress = memStart;
		memEnd = memStart + _maxSize;

		return true;
	}

	void StackAllocator::cleanUp()
	{
		free(memStart);
	}

	void StackAllocator::reset()
	{
		currentAddress = memStart;
	}

}
