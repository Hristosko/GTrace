#pragma once

#pragma once

#include "MemoryBench.h"

/**
 * Objects that can be allocated on the heap shoud inherit this class.
 * This way the memory usage will be tracked by the memory bench.
 * See HeapAligned.h as well
 */
class HeapAlocated {
public:
	void* operator new(size_t size) {
		void* res = ::operator new(size);
		MemoryBench::allocate(res);
		return res;
	}
	void operator delete(void* ptr) {
		MemoryBench::free(ptr);
		::operator delete(ptr);
	}
};