#include "MemoryBench.h"

#include <algorithm> // std:::max
#include <mutex> //std::mutex

namespace MemoryBench {

#ifdef GTRACE_MEMORY_BENCH

struct InternalData {
	Counter currentAllocatedMemory;
	Counter peakAllocatedMemory;
	Counter totalAllocatedMemory;
	Counter allocationCount;
	Counter freeCount;
	std::mutex mu;
};

static InternalData id;

/**
 * Return the size of the allocated memory.
 * For Windows this will return the requested memory,
 * for Linux - the actual size on the heap (not tested here)
 * TODO: implement both for windows at least
 */
static Counter getHeapMemory(void* ptr) {
#ifdef _WIN32
	return _msize(ptr);
#endif // _WIN32
#ifdef __linux__
	return *(reinterpret_cast<Counter*>(ptr) - 1);
#endif // __linux__
}

/**
 * Same as getHeapMemory(void* ptr) but for aligned allocation,
 * see HeapAligned.h
 */
static Counter getHeapMemory(void* ptr, size_t alignment) {
#ifdef _WIN32
	return _aligned_msize(ptr, alignment, 0 /* the whole struct is alligned like this */);
#endif // _WIN32
#ifdef __linux__
	return *(reinterpret_cast<Counter*>(ptr) - 1);
#endif // __linux__
}

/**
 * Initializes the internal data used by the benchmark. 
 */
void init() {
	id.currentAllocatedMemory = 0;
	id.peakAllocatedMemory = 0;
	id.totalAllocatedMemory = 0;
	id.allocationCount = 0;
	id.freeCount = 0;
}

/**
 * Get the currently accumulated data.
 */
Data get() {
	return { id.totalAllocatedMemory, id.peakAllocatedMemory, id.allocationCount , id.freeCount };
}

/**
 * Accumulates single allocation with the given size. 
 */
static void increaseMemory(Counter size) {
	std::lock_guard<std::mutex> lock(id.mu);
	++id.allocationCount;
	id.currentAllocatedMemory += size;
	id.totalAllocatedMemory += size;
	id.peakAllocatedMemory = std::max(id.peakAllocatedMemory, id.currentAllocatedMemory);
}

/**
 * Accumulates single mem. free with the given size.
 */
static void decreaseMemory(Counter size) {
	std::lock_guard<std::mutex> lock(id.mu);
	++id.freeCount;
	id.currentAllocatedMemory -= size;
}

void allocate(void* ptr) {
	const Counter size = getHeapMemory(ptr);
	increaseMemory(size);
}

void free(void* ptr) {
	const Counter size = getHeapMemory(ptr);
	decreaseMemory(size);
}
void allocate(void* ptr, size_t alignment) {
	const Counter size = getHeapMemory(ptr, alignment);
	increaseMemory(size);
}
void free(void* ptr, size_t alignment) {
	const Counter size = getHeapMemory(ptr, alignment);
	decreaseMemory(size);
}

#else

void init() {}

void allocate(void* ptr) {}
void free(void* ptr) {}

void allocate(void* ptr, size_t alignment) {}
void free(void* ptr, size_t alignment) {}

Data get() {
	return { 0,0,0,0 };
}

#endif //GTRACE_MEMORY_BENCH
}