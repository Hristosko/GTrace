#pragma once

#include "MemoryBench.h"
#include <memory>

template <typename T>
T* allocateBuffer(size_t count) {
	T* ptr = new T[count];
	MemoryBench::allocate(ptr);
	return ptr;
}

template <typename T>
void deleteBuffer(T* ptr) {
	MemoryBench::free(ptr);
	delete[] ptr;
}

template <typename T>
struct UniqueBufferPtrDeleter {
	void operator()(T* ptr) { deleteBuffer(ptr); }
};

template<typename T>
using UniqueBufferPtr = std::unique_ptr<T[], UniqueBufferPtrDeleter<T>>;

template <typename T>
UniqueBufferPtr<T> makeUniqueBuffer(size_t size) {
	return UniqueBufferPtr<T>(allocateBuffer<T>(size));
}