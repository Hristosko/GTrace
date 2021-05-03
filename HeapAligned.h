#pragma once

#include "MemoryBench.h"

/**
 * Objects that require extra aligment (Vector3f) should inherit this class.
 */
template<size_t N>
class HeapAligned {
public:
	void* operator new(size_t size) {
		void* res = ::operator new(size, std::align_val_t(N));
		MemoryBench::allocate(res, N);
		return res;
	}
	void operator delete(void* ptr) {
		MemoryBench::free(ptr, N);
		::operator delete(ptr, std::align_val_t(N));
	}
};