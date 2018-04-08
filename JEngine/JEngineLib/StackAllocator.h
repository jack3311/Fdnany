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

		template <typename T, typename ... Args>
		T * allocateRaw(Args ... _args);

		void reset();
	};

	template<typename T, typename ... Args>
	inline T * StackAllocator::allocateRaw(Args ... _args)
	{
		size_t size = sizeof(T);

		if (currentAddress + size > memEnd)
		{
			throw new std::exception();
		}

		T * allocated = reinterpret_cast<T *>(currentAddress);

		currentAddress += size;

		//Call constructor manually
		new (allocated) T(_args...);

		return allocated;
	}
}