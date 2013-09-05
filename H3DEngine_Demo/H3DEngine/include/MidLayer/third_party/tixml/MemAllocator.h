#ifndef __MEMALLOCATOR_H__uuskqpiw93__
#define __MEMALLOCATOR_H__uuskqpiw93__

#include <stdlib.h>
#include <Windows.h>
#include <process.h>

struct CBiboFakeLock
{
	void Lock(){}
	void Unlock(){}
};
class CBiboLock
{
private:
	CRITICAL_SECTION m_sect;
public:
	CBiboLock()
	{
		InitializeCriticalSection(&m_sect);
	}
	~CBiboLock()
	{
		DeleteCriticalSection(&m_sect);
	}
	void Lock()
	{
		EnterCriticalSection(&m_sect);
	}
	void Unlock()
	{
		LeaveCriticalSection(&m_sect);
	}
};

template<class T>
class CBiboScopedLock
{
public:
	CBiboScopedLock(T* l):m_lock(l){m_lock->Lock();}
	~CBiboScopedLock(){m_lock->Unlock();}
private:
	T* m_lock;
};

template<class T, class LockMode=CBiboFakeLock>
class ClassPool
{
	//NOTE!!! needed better low level allocator, to replace new operator

	int numAllocated; ///< number of elements currently allocated through this ClassPool
	size_t elementSize; ///< the size of each element, or the size of a pointer, whichever is greater
	T *freeListHead; ///< a pointer to a linked list of freed elements for reuse

	LockMode m_lock;
public:
	ClassPool()
	{
		numAllocated = 0;
		elementSize = sizeof(T)>sizeof(T *)?sizeof(T):sizeof(T *);
		freeListHead = NULL;
	}
	~ClassPool()
	{
		while(freeListHead)
		{
			T* ret = freeListHead;
			freeListHead = *(reinterpret_cast<T**>(freeListHead));
			::free(ret);
		}
	}
	int GetCount()const
	{
		return numAllocated;
	}

	void SnapShot(int lv,unsigned int verbose)const;
	/// Allocates and properly constructs in place a new element.
	T *alloc()
	{
		CBiboScopedLock<LockMode> lock(&m_lock);

		numAllocated++;
		if(freeListHead == NULL)
			return new(malloc(elementSize))T;
		T* ret = freeListHead;
		freeListHead = *(reinterpret_cast<T**>(freeListHead));
		return new(ret)T;
	}
	template<class P>
	T *alloc(const P& p)
	{
		CBiboScopedLock<LockMode> lock(&m_lock);

		numAllocated++;
		if(freeListHead == NULL)
			return new(malloc(elementSize))T(p);
		T* ret = freeListHead;
		freeListHead = *(reinterpret_cast<T**>(freeListHead));
		return new(ret)T(p);
	}
	template<class P1, class P2>
	T *alloc(const P1& p1, const P2& p2)
	{
		CBiboScopedLock<LockMode> lock(&m_lock);

		numAllocated++;
		if(freeListHead == NULL)
			return new(malloc(elementSize))T(p1, p2);
		T* ret = freeListHead;
		freeListHead = *(reinterpret_cast<T**>(freeListHead));
		return new(ret)T(p1, p2);
	}
	template<class P1, class P2, class P3>
	T *alloc(const P1& p1, const P2& p2, const P3& p3)
	{
		CBiboScopedLock<LockMode> lock(&m_lock);

		numAllocated++;
		if(freeListHead == NULL)
			return new(malloc(elementSize))T(p1, p2, p3);
		T* ret = freeListHead;
		freeListHead = *(reinterpret_cast<T**>(freeListHead));
		return new(ret)T(p1, p2, p3);
	}
	template<class P1, class P2, class P3, class P4>
	T *alloc(const P1& p1, const P2& p2, const P3& p3, const P4& p4)
	{
		CBiboScopedLock<LockMode> lock(&m_lock);

		numAllocated++;
		if(freeListHead == NULL)
			return new(malloc(elementSize))T(p1, p2, p3, p4);
		T* ret = freeListHead;
		freeListHead = *(reinterpret_cast<T**>(freeListHead));
		return new(ret)T(p1, p2, p3, p4);
	}
	template<class P1, class P2, class P3, class P4, class P5>
	T *alloc(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5)
	{
		CBiboScopedLock<LockMode> lock(&m_lock);

		numAllocated++;
		if(freeListHead == NULL)
			return new(malloc(elementSize))T(p1, p2, p3, p4, p5);
		T* ret = freeListHead;
		freeListHead = *(reinterpret_cast<T**>(freeListHead));
		return new(ret)T(p1, p2, p3, p4, p5);
	}
	template<class P1, class P2, class P3, class P4, class P5, class P6>
	T *alloc(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6)
	{
		CBiboScopedLock<LockMode> lock(&m_lock);

		numAllocated++;
		if(freeListHead == NULL)
			return new(malloc(elementSize))T(p1, p2, p3, p4, p5, p6);
		T* ret = freeListHead;
		freeListHead = *(reinterpret_cast<T**>(freeListHead));
		return new(ret)T(p1, p2, p3, p4, p5, p6);
	}
	template<class P1, class P2, class P3, class P4, class P5, class P6, class P7>
	T *alloc(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7)
	{
		CBiboScopedLock<LockMode> lock(&m_lock);

		numAllocated++;
		if(freeListHead == NULL)
			return new(malloc(elementSize))T(p1, p2, p3, p4, p5, p6, p7);
		T* ret = freeListHead;
		freeListHead = *(reinterpret_cast<T**>(freeListHead));
		return new(ret)T(p1, p2, p3, p4, p5, p6, p7);
	}
	template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
	T *alloc(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8)
	{
		CBiboScopedLock<LockMode> lock(&m_lock);

		numAllocated++;
		if(freeListHead == NULL)
			return new(malloc(elementSize))T(p1, p2, p3, p4, p5, p6, p7, p8);
		T* ret = freeListHead;
		freeListHead = *(reinterpret_cast<T**>(freeListHead));
		return new(ret)T(p1, p2, p3, p4, p5, p6, p7, p8);
	}
	template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
	T *alloc(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, const P9& p9)
	{
		CBiboScopedLock<LockMode> lock(&m_lock);

		numAllocated++;
		if(freeListHead == NULL)
			return new(malloc(elementSize))T(p1, p2, p3, p4, p5, p6, p7, p8, p9);
		T* ret = freeListHead;
		freeListHead = *(reinterpret_cast<T**>(freeListHead));
		return new(ret)T(p1, p2, p3, p4, p5, p6, p7, p8, p9);
	}
	template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10>
	T *alloc(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, const P9& p9, const P10& p10)
	{
		CBiboScopedLock<LockMode> lock(&m_lock);

		numAllocated++;
		if(freeListHead == NULL)
			return new(malloc(elementSize))T(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
		T* ret = freeListHead;
		freeListHead = *(reinterpret_cast<T**>(freeListHead));
		return new(ret)T(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
	}

	/// Properly destructs and frees an element allocated with the alloc method.
	void dealloc(T* elem)
	{
		elem->~T();

#ifdef MEM_LEAK_CHECK
		::free(elem);
#else
		if(true)
		{
			CBiboScopedLock<LockMode> lock(&m_lock);
#if _DEBUG
			memset(elem, 0xfe, elementSize);
#endif
			numAllocated--;
//			lightAssert(numAllocated>=0);
			*(reinterpret_cast<T**>(elem)) = freeListHead;
			freeListHead = elem;
		}
#endif
	}
};

#endif
