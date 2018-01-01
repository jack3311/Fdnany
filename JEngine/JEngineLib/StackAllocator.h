#pragma once

namespace JEngine
{
	class StackAllocator
	{
	private:
		char * memStart;
		size_t maxSize;
		char * memEnd;

		char * currentAddress;

	public:
		StackAllocator();
		StackAllocator(const StackAllocator &) = delete;
		~StackAllocator();

		bool initialise(size_t _maxSize);
		void cleanUp();

		template <typename T>
		T * allocateRaw();

		void reset();
	};

	template<typename T>
	inline T * StackAllocator::allocateRaw()
	{
		size_t size = sizeof(T);

		if (currentAddress + size > memEnd)
		{
			throw new std::exception();
		}

		T * allocated = reinterpret_cast<T *>(currentAddress);

		currentAddress += size;

		//Call constructor manually
		new (allocated) T();

		return allocated;
	}
}