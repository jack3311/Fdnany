#pragma once

#include <stdlib.h>
#include <queue>
#include <memory>

namespace JEngine
{
	template <typename T>
	__interface PoolAllocatorParent //Use this to avoid pool_alloc_pointer having a maxCount template param
	{
		void deallocate(T *);
	};

	template <typename T>
	class pool_alloc_pointer
	{
	private:
		PoolAllocatorParent<T> * poolAllocator;
		unsigned int * refCount;
		T * allocated;

		inline void checkRefCount();

	public:
		pool_alloc_pointer(); //Unassigned
		pool_alloc_pointer(T *, PoolAllocatorParent<T> *); //New
		pool_alloc_pointer(const pool_alloc_pointer<T> &);
		~pool_alloc_pointer();

		T & operator*();
		pool_alloc_pointer<T> * operator->();

		pool_alloc_pointer<T> & operator=(const pool_alloc_pointer<T> &); //Copy assignment
		pool_alloc_pointer<T> & operator=(const pool_alloc_pointer<T> &&); //Move assignment
	};

	template <typename T, unsigned int maxCount>
	class PoolAllocator : public PoolAllocatorParent<T>
	{
	private:
		char * memStart;

		std::queue<char *> availableAllocationQueue;

		std::shared_ptr<PoolAllocator<unsigned int, maxCount> refCounterPool; //Creates ref counters for child pointers

	public:
		PoolAllocator();
		PoolAllocator(const PoolAllocator &) = delete;
		~PoolAllocator();

		bool initialise();
		void cleanUp();

		T * allocateRaw();
		pool_alloc_pointer<T> allocate();


		void deallocate(T *);

		//void reset();
	};



	template<typename T, unsigned int maxCount>
	PoolAllocator<T, maxCount>::PoolAllocator()
	{
	}

	template<typename T, unsigned int maxCount>
	PoolAllocator<T, maxCount>::~PoolAllocator()
	{
	}

	template<typename T, unsigned int maxCount>
	bool PoolAllocator<T, maxCount>::initialise()
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

		//Initialise ref counter pool
		ERR_IF(!refCounterPool.initialise(), "Could not initialise ref counter pool");

		return true;
	}

	template<typename T, unsigned int maxCount>
	void PoolAllocator<T, maxCount>::cleanUp()
	{
		//Clean up ref counter pool
		refCounterPool.cleanUp();

		//Clean up this pool
		free(memStart);
	}

	template<typename T, unsigned int maxCount>
	inline T * PoolAllocator<T, maxCount>::allocateRaw()
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
	inline pool_alloc_pointer<T> PoolAllocator<T, maxCount>::allocate()
	{
		return pool_alloc_pointer<T>(allocateRaw(), this);
	}

	template<typename T, unsigned int maxCount>
	inline void PoolAllocator<T, maxCount>::deallocate(T * _allocated)
	{
		_allocated->~T();
		availableAllocationQueue.push(reinterpret_cast<char *>(_allocated));
	}



	

	template<typename T>
	inline void pool_alloc_pointer<T>::checkRefCount()
	{
		if (allocated && --(*refCount) == 0)
		{
			//Deallocate
			poolAllocator->deallocate(allocated);
			delete refCount;
		}
	}

	template<typename T>
	inline pool_alloc_pointer<T>::pool_alloc_pointer() : 
		poolAllocator(nullptr), refCount(nullptr), allocated(nullptr)
	{
	}

	template<typename T>
	inline pool_alloc_pointer<T>::pool_alloc_pointer(T * _allocated, PoolAllocatorParent<T> * _poolAllocator) : //New
		allocated(_allocated), poolAllocator(_poolAllocator)
	{
		refCount = new unsigned int(1);
	}

	template<typename T>
	inline pool_alloc_pointer<T>::pool_alloc_pointer(const pool_alloc_pointer<T> & _other)
	{
		allocated = _other.allocated;
		refCount = _other.refCount;
		poolAllocator = _other.poolAllocator;

		//Increment ref counter
		++(*refCount);
	}

	template<typename T>
	inline pool_alloc_pointer<T>::~pool_alloc_pointer()
	{
		checkRefCount();
	}

	template<typename T>
	inline T & pool_alloc_pointer<T>::operator*()
	{
		if (allocated)
			return *allocated;
		else
			throw new std::exception("Error: cannot dereference an unassigned pool_alloc_pointer");
	}

	template<typename T>
	inline pool_alloc_pointer<T> * pool_alloc_pointer<T>::operator->()
	{
		if (allocated)
			return allocated;
		else
			throw new std::exception("Error: cannot dereference an unassigned pool_alloc_pointer");
	}

	template<typename T>
	inline pool_alloc_pointer<T> & pool_alloc_pointer<T>::operator=(const pool_alloc_pointer<T> & _other) //Copy assignment
	{
		if (allocated != _other.allocated)
		{
			checkRefCount();

			allocated = _other.allocated;
			refCount = _other.refCount;
			poolAllocator = _other.poolAllocator;

			//Increment ref counter
			++(*refCount);
		}

		return *this;
	}

	template<typename T>
	inline pool_alloc_pointer<T> & pool_alloc_pointer<T>::operator=(const pool_alloc_pointer<T> && _other) //Move assignment
	{
		if (allocated != _other.allocated)
		{
			checkRefCount();

			allocated = _other.allocated;
			refCount = _other.refCount;
			poolAllocator = _other.poolAllocator;

			//Increment ref counter
			++(*refCount);
		}

		return *this;
	}
}