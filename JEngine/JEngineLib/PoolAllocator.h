#pragma once

#include <stdlib.h>
#include <queue>
#include <memory>

namespace JEngine
{
	template <unsigned int maxSize, unsigned int maxCount>
	class RcPoolAllocator;
	
	template <typename T, unsigned int maxSize, unsigned int maxCount>
	class pool_alloc_pointer
	{
	private:
		RcPoolAllocator<maxSize, maxCount> * rcPoolAllocator;
		unsigned int * refCount;
		T * allocated;

		inline void checkRefCount();

	public:
		pool_alloc_pointer(); //Unassigned
		pool_alloc_pointer(T *, RcPoolAllocator<maxSize, maxCount> *); //New
		pool_alloc_pointer(const pool_alloc_pointer<T, maxSize, maxCount> &);
		pool_alloc_pointer(T *, RcPoolAllocator<maxSize, maxCount> *, unsigned int *);
		~pool_alloc_pointer();

		T & operator*();
		pool_alloc_pointer<T, maxSize, maxCount> * operator->();

		pool_alloc_pointer<T, maxSize, maxCount> & operator=(const pool_alloc_pointer<T, maxSize, maxCount> &); //Copy assignment
		pool_alloc_pointer<T, maxSize, maxCount> & operator=(const pool_alloc_pointer<T, maxSize, maxCount> &&); //Move assignment

		template <typename Tar>
		operator pool_alloc_pointer<Tar, maxSize, maxCount>();
	};

	template <unsigned int maxSize, unsigned int maxCount>
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

		template <typename T, typename... Ts>
		T * allocateRaw(Ts...);

		template <typename T>
		void deallocate(T *);
	};

	template <unsigned int maxSize, unsigned int maxCount>
	class RcPoolAllocator : public RawPoolAllocator<maxSize, maxCount>
	{
	public:
		std::shared_ptr<RawPoolAllocator<sizeof(unsigned int), maxCount>> refCounterPool; //Creates ref counters for child pointers

	public:
		RcPoolAllocator();
		RcPoolAllocator(const RcPoolAllocator &) = delete;
		~RcPoolAllocator();

		bool initialise();
		void cleanUp();

		template<typename T, typename ...Ts>
		pool_alloc_pointer<T, maxSize, maxCount> allocate(Ts...);
	};



	template<unsigned int maxSize, unsigned int maxCount>
	RawPoolAllocator<maxSize, maxCount>::RawPoolAllocator()
	{
	}

	template<unsigned int maxSize, unsigned int maxCount>
	RawPoolAllocator<maxSize, maxCount>::~RawPoolAllocator()
	{
	}

	template<unsigned int maxSize, unsigned int maxCount>
	bool RawPoolAllocator<maxSize, maxCount>::initialise()
	{
		//Allocate once
		void * mallocRes = malloc(maxCount * maxSize);

		if (mallocRes == NULL)
		{
			throw std::exception("Error: Malloc failed");
		}

		memStart = reinterpret_cast<char *>(mallocRes);

		//Populate queue
		for (unsigned int i = 0; i < maxCount; ++i)
		{
			availableAllocationQueue.push(memStart + i * maxSize);
		}

		return true;
	}

	template<unsigned int maxSize, unsigned int maxCount>
	void RawPoolAllocator<maxSize, maxCount>::cleanUp()
	{
		//Clean up this pool
		free(memStart);
	}

	template<unsigned int maxSize, unsigned int maxCount>
	template<typename T, typename ...Ts>
	inline T * RawPoolAllocator<maxSize, maxCount>::allocateRaw(Ts... _args)
	{
		static_assert(sizeof(T) <= maxSize, "Object is too big for this allocator");

		if (availableAllocationQueue.empty())
		{
			throw new std::exception("Error: no more allocations available");
		}

		char * allocated = availableAllocationQueue.front();
		availableAllocationQueue.pop();

		//Call constructor manually
		new (allocated) T(_args...);

		return reinterpret_cast<T *>(allocated);
	}

	template<unsigned int maxSize, unsigned int maxCount>
	template <typename T>
	inline void RawPoolAllocator<maxSize, maxCount>::deallocate(T * _allocated)
	{
		_allocated->~T();
		availableAllocationQueue.push(reinterpret_cast<char *>(_allocated));
	}




	template<unsigned int maxSize, unsigned int maxCount>
	inline RcPoolAllocator<maxSize, maxCount>::RcPoolAllocator()
	{
		refCounterPool = std::make_shared<RawPoolAllocator<sizeof(unsigned int), maxCount>>();
	}

	template<unsigned int maxSize, unsigned int maxCount>
	inline RcPoolAllocator<maxSize, maxCount>::~RcPoolAllocator()
	{
		RawPoolAllocator::~RawPoolAllocator();
	}

	template<unsigned int maxSize, unsigned int maxCount>
	inline bool RcPoolAllocator<maxSize, maxCount>::initialise()
	{
		//Initialise ref counter pool
		ERR_IF(!refCounterPool->initialise(), "Could not initialise ref counter pool allocator", "Initialised ref counter pool allocator");
		return RawPoolAllocator::initialise();
	}

	template<unsigned int maxSize, unsigned int maxCount>
	inline void RcPoolAllocator<maxSize, maxCount>::cleanUp()
	{
		//Clean up ref counter pool
		refCounterPool->cleanUp();

		//Clean up this pool
		RawPoolAllocator::cleanUp();
	}

	template<unsigned int maxSize, unsigned int maxCount>
	template<typename T, typename ...Ts>
	inline pool_alloc_pointer<T, maxSize, maxCount> RcPoolAllocator<maxSize, maxCount>::allocate(Ts... _args)
	{
		return pool_alloc_pointer<T, maxSize, maxCount>(allocateRaw<T>(_args...), this);
	}



	
	template<typename T, unsigned int maxSize, unsigned int maxCount>
	inline void pool_alloc_pointer<T, maxSize, maxCount>::checkRefCount()
	{
		if (allocated && --(*refCount) == 0)
		{
			//Deallocate
			rcPoolAllocator->deallocate(allocated);
			rcPoolAllocator->refCounterPool->deallocate(refCount);
		}
	}

	template<typename T, unsigned int maxSize, unsigned int maxCount>
	inline pool_alloc_pointer<T, maxSize, maxCount>::pool_alloc_pointer() :
		rcPoolAllocator(nullptr), refCount(nullptr), allocated(nullptr)
	{
	}

	template<typename T, unsigned int maxSize, unsigned int maxCount>
	inline pool_alloc_pointer<T, maxSize, maxCount>::pool_alloc_pointer(T * _allocated, RcPoolAllocator<maxSize, maxCount> * _rcPoolAllocator) : //New
		allocated(_allocated), rcPoolAllocator(_rcPoolAllocator)
	{
		refCount = rcPoolAllocator->refCounterPool->allocateRaw<unsigned int>(1u);
		//*refCount = 1;

	}

	template<typename T, unsigned int maxSize, unsigned int maxCount>
	inline pool_alloc_pointer<T, maxSize, maxCount>::pool_alloc_pointer(const pool_alloc_pointer<T, maxSize, maxCount> & _other)
	{
		allocated = _other.allocated;
		refCount = _other.refCount;
		rcPoolAllocator = _other.rcPoolAllocator;

		//Increment ref counter
		++(*refCount);
	}

	template<typename T, unsigned int maxSize, unsigned int maxCount>
	inline pool_alloc_pointer<T, maxSize, maxCount>::pool_alloc_pointer(T * _allocated, RcPoolAllocator<maxSize, maxCount> * _rcPoolAllocator, unsigned int * _refCount) :
		allocated(_allocated), rcPoolAllocator(_rcPoolAllocator), refCount(_refCount)
	{
	}

	template<typename T, unsigned int maxSize, unsigned int maxCount>
	inline pool_alloc_pointer<T, maxSize, maxCount>::~pool_alloc_pointer()
	{
		checkRefCount();
	}

	template<typename T, unsigned int maxSize, unsigned int maxCount>
	inline T & pool_alloc_pointer<T, maxSize, maxCount>::operator*()
	{
		if (allocated)
			return *allocated;
		else
			throw new std::exception("Error: cannot dereference an unassigned pool_alloc_pointer");
	}

	template<typename T, unsigned int maxSize, unsigned int maxCount>
	inline pool_alloc_pointer<T, maxSize, maxCount> * pool_alloc_pointer<T, maxSize, maxCount>::operator->()
	{
		if (allocated)
			return allocated;
		else
			throw new std::exception("Error: cannot dereference an unassigned pool_alloc_pointer");
	}

	template<typename T, unsigned int maxSize, unsigned int maxCount>
	inline pool_alloc_pointer<T, maxSize, maxCount> & pool_alloc_pointer<T, maxSize, maxCount>::operator=(const pool_alloc_pointer<T, maxSize, maxCount> & _other) //Copy assignment
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

	template<typename T, unsigned int maxSize, unsigned int maxCount>
	inline pool_alloc_pointer<T, maxSize, maxCount> & pool_alloc_pointer<T, maxSize, maxCount>::operator=(const pool_alloc_pointer<T, maxSize, maxCount> && _other) //Move assignment
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
	
	template<typename T, unsigned int maxSize, unsigned int maxCount>
	template<typename Tar>
	inline pool_alloc_pointer<T, maxSize, maxCount>::operator pool_alloc_pointer<Tar, maxSize, maxCount>()
	{
		static_assert(std::is_base_of<Tar, T>::value, "Cannot convert pool_alloc_pointer to non-parent type");

		pool_alloc_pointer<Tar, maxSize, maxCount> converted{
			dynamic_cast<Tar *>(allocated),
			rcPoolAllocator,
			refCount
		};

		++(*refCount);

		return converted;
	}
}