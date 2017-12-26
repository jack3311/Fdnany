#pragma once

#include <stdlib.h>
#include <queue>
#include <memory>

namespace JEngine
{
	template <typename T, unsigned int maxCount>
	class RcPoolAllocator;

	template <typename T, unsigned int maxCount>
	class pool_alloc_pointer
	{
	private:
		RcPoolAllocator<T, maxCount> * rcPoolAllocator;
		unsigned int * refCount;
		T * allocated;

		inline void checkRefCount();

	public:
		pool_alloc_pointer(); //Unassigned
		pool_alloc_pointer(T *, RcPoolAllocator<T, maxCount> *); //New
		pool_alloc_pointer(const pool_alloc_pointer<T, maxCount> &);
		~pool_alloc_pointer();

		T & operator*();
		pool_alloc_pointer<T, maxCount> * operator->();

		pool_alloc_pointer<T, maxCount> & operator=(const pool_alloc_pointer<T, maxCount> &); //Copy assignment
		pool_alloc_pointer<T, maxCount> & operator=(const pool_alloc_pointer<T, maxCount> &&); //Move assignment
	};

	template <typename T, unsigned int maxCount>
	class RawPoolAllocator
	{
	private:
		char * memStart;

		std::queue<char *> availableAllocationQueue;

	public:
		RawPoolAllocator();
		RawPoolAllocator(const RawPoolAllocator &) = delete;
		~RawPoolAllocator();

		bool initialise();
		void cleanUp();

		T * allocateRaw();
		void deallocate(T *);
	};

	template <typename T, unsigned int maxCount>
	class RcPoolAllocator : public RawPoolAllocator<T, maxCount>
	{
	private:
	public: //TODO: <-- WHY ISN'T THE GETTER BEING INLINED!?!?!?!?
		std::shared_ptr<RawPoolAllocator<unsigned int, maxCount>> refCounterPool; //Creates ref counters for child pointers

	public:
		RcPoolAllocator();
		RcPoolAllocator(const RcPoolAllocator &) = delete;
		~RcPoolAllocator();

		bool initialise();
		void cleanUp();

		pool_alloc_pointer<T, maxCount> allocate();

		std::shared_ptr<RawPoolAllocator<unsigned int, maxCount>> getRefCounterPool();
	};



	template<typename T, unsigned int maxCount>
	RawPoolAllocator<T, maxCount>::RawPoolAllocator()
	{
	}

	template<typename T, unsigned int maxCount>
	RawPoolAllocator<T, maxCount>::~RawPoolAllocator()
	{
	}

	template<typename T, unsigned int maxCount>
	bool RawPoolAllocator<T, maxCount>::initialise()
	{
		//Allocate once
		void * mallocRes = malloc(maxCount * sizeof(T));

		if (mallocRes == NULL)
		{
			throw std::exception("Error: Malloc failed");
		}

		memStart = reinterpret_cast<char *>(mallocRes);

		//Populate queue
		for (unsigned int i = 0; i < maxCount; ++i)
		{
			availableAllocationQueue.push(memStart + i * sizeof(T));
		}

		return true;
	}

	template<typename T, unsigned int maxCount>
	void RawPoolAllocator<T, maxCount>::cleanUp()
	{
		//Clean up this pool
		free(memStart);
	}

	template<typename T, unsigned int maxCount>
	inline T * RawPoolAllocator<T, maxCount>::allocateRaw()
	{
		if (availableAllocationQueue.empty())
		{
			throw new std::exception("Error: no more allocations available");
		}

		char * allocated = availableAllocationQueue.front();
		availableAllocationQueue.pop();

		//Call constructor manually
		new (allocated) T();

		return reinterpret_cast<T *>(allocated);
	}

	template<typename T, unsigned int maxCount>
	inline void RawPoolAllocator<T, maxCount>::deallocate(T * _allocated)
	{
		_allocated->~T();
		availableAllocationQueue.push(reinterpret_cast<char *>(_allocated));
	}




	template<typename T, unsigned int maxCount>
	inline RcPoolAllocator<T, maxCount>::RcPoolAllocator()
	{
		refCounterPool = std::make_shared<RawPoolAllocator<unsigned int, maxCount>>();
	}

	template<typename T, unsigned int maxCount>
	inline RcPoolAllocator<T, maxCount>::~RcPoolAllocator()
	{
		//RawPoolAllocator::~RawPoolAllocator();
	}

	template<typename T, unsigned int maxCount>
	inline bool RcPoolAllocator<T, maxCount>::initialise()
	{
		//Initialise ref counter pool
		ERR_IF(!refCounterPool->initialise(), "Could not initialise ref counter pool");
		return RawPoolAllocator::initialise();
	}

	template<typename T, unsigned int maxCount>
	inline void RcPoolAllocator<T, maxCount>::cleanUp()
	{
		//Clean up ref counter pool
		refCounterPool->cleanUp();

		//Clean up this pool
		RawPoolAllocator::cleanUp();
	}

	template<typename T, unsigned int maxCount>
	inline pool_alloc_pointer<T, maxCount> RcPoolAllocator<T, maxCount>::allocate()
	{
		return pool_alloc_pointer<T, maxCount>(allocateRaw(), this);
	}

	template<typename T, unsigned int maxCount>
	inline std::shared_ptr<RawPoolAllocator<unsigned int, maxCount>> RcPoolAllocator<T, maxCount>::getRefCounterPool()
	{
		return refCounterPool;
	}



	
	template<typename T, unsigned int maxCount>
	inline void pool_alloc_pointer<T, maxCount>::checkRefCount()
	{
		if (allocated && --(*refCount) == 0)
		{
			//Deallocate
			rcPoolAllocator->deallocate(allocated);
			rcPoolAllocator->refCounterPool->deallocate(refCount);
			//rcPoolAllocator->getRefCounterPool()->deallocate(refCount);
			//delete refCount;
		}
	}

	template<typename T, unsigned int maxCount>
	inline pool_alloc_pointer<T, maxCount>::pool_alloc_pointer() :
		rcPoolAllocator(nullptr), refCount(nullptr), allocated(nullptr)
	{
	}

	template<typename T, unsigned int maxCount>
	inline pool_alloc_pointer<T, maxCount>::pool_alloc_pointer(T * _allocated, RcPoolAllocator<T, maxCount> * _rcPoolAllocator) : //New
		allocated(_allocated), rcPoolAllocator(_rcPoolAllocator)
	{
		//refCount = new unsigned int(1);
		//refCount = rcPoolAllocator->getRefCounterPool()->allocateRaw();
		refCount = rcPoolAllocator->refCounterPool->allocateRaw();
		*refCount = 1;

	}

	template<typename T, unsigned int maxCount>
	inline pool_alloc_pointer<T, maxCount>::pool_alloc_pointer(const pool_alloc_pointer<T, maxCount> & _other)
	{
		allocated = _other.allocated;
		refCount = _other.refCount;
		rcPoolAllocator = _other.rcPoolAllocator;

		//Increment ref counter
		++(*refCount);
	}

	template<typename T, unsigned int maxCount>
	inline pool_alloc_pointer<T, maxCount>::~pool_alloc_pointer()
	{
		checkRefCount();
	}

	template<typename T, unsigned int maxCount>
	inline T & pool_alloc_pointer<T, maxCount>::operator*()
	{
		if (allocated)
			return *allocated;
		else
			throw new std::exception("Error: cannot dereference an unassigned pool_alloc_pointer");
	}

	template<typename T, unsigned int maxCount>
	inline pool_alloc_pointer<T, maxCount> * pool_alloc_pointer<T, maxCount>::operator->()
	{
		if (allocated)
			return allocated;
		else
			throw new std::exception("Error: cannot dereference an unassigned pool_alloc_pointer");
	}

	template<typename T, unsigned int maxCount>
	inline pool_alloc_pointer<T, maxCount> & pool_alloc_pointer<T, maxCount>::operator=(const pool_alloc_pointer<T, maxCount> & _other) //Copy assignment
	{
		if (allocated != _other.allocated)
		{
			checkRefCount();

			allocated = _other.allocated;
			refCount = _other.refCount;
			rcPoolAllocator = _other.rcPoolAllocator;

			//Increment ref counter
			++(*refCount);
		}

		return *this;
	}

	template<typename T, unsigned int maxCount>
	inline pool_alloc_pointer<T, maxCount> & pool_alloc_pointer<T, maxCount>::operator=(const pool_alloc_pointer<T, maxCount> && _other) //Move assignment
	{
		if (allocated != _other.allocated)
		{
			checkRefCount();

			allocated = _other.allocated;
			refCount = _other.refCount;
			rcPoolAllocator = _other.rcPoolAllocator;

			//Increment ref counter
			++(*refCount);
		}

		return *this;
	}
}