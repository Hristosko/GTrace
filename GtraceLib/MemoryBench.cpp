#include "MemoryBench.h"

#include <algorithm> // std:::max
#include <atomic>

namespace MemoryBench {

#ifdef GTRACE_MEMORY_BENCH

namespace gtrace {

struct InternalData {
	std::atomic<Counter> currentAllocatedMemory;
	std::atomic<Counter> peakAllocatedMemory;
	std::atomic<Counter> totalAllocatedMemory;
	std::atomic<Counter> allocationCount;
	std::atomic<Counter> freeCount;

	InternalData() :
		currentAllocatedMemory(0),
		peakAllocatedMemory(0),
		totalAllocatedMemory(0),
		allocationCount(0),
		freeCount(0) {}
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
 * Get the currently accumulated data.
 */
Data get() {
	return { id.totalAllocatedMemory, id.peakAllocatedMemory, id.allocationCount , id.freeCount };
}

/**
 * Reset all accumulators to 0
 */
void reset() {
	id.currentAllocatedMemory.store(0, std::memory_order_relaxed);
	id.peakAllocatedMemory.store(0, std::memory_order_relaxed);
	id.totalAllocatedMemory.store(0, std::memory_order_relaxed);
	id.allocationCount.store(0, std::memory_order_relaxed);
	id.freeCount.store(0, std::memory_order_relaxed);
}

/**
 * Accumulates single allocation with the given size.
 */
static void increaseMemory(Counter size) {
	id.allocationCount.fetch_add(1, std::memory_order_relaxed);
	id.currentAllocatedMemory.fetch_add(size, std::memory_order_relaxed);

	Counter peak = id.peakAllocatedMemory.load(std::memory_order_relaxed);
	Counter cur = id.currentAllocatedMemory.load(std::memory_order_relaxed);
	while (id.currentAllocatedMemory.load(std::memory_order_relaxed) <= cur
		&& peak < cur) {
		id.peakAllocatedMemory.compare_exchange_weak(peak, cur, std::memory_order_relaxed, std::memory_order_relaxed);
	}

	// This isn't threadsafe, but should be accurate enough
	if (cur > peak) id.peakAllocatedMemory.store(cur, std::memory_order_relaxed);

	id.totalAllocatedMemory.fetch_add(size, std::memory_order_relaxed);
}

/**
 * Accumulates single mem. free with the given size.
 */
static void decreaseMemory(Counter size) {
	id.freeCount.fetch_add(1, std::memory_order_relaxed);
	id.currentAllocatedMemory.fetch_sub(size, std::memory_order_relaxed);
}

static void allocateMemory(void* ptr) {
	const Counter size = getHeapMemory(ptr);
	increaseMemory(size);
}

static void freeMemory(void* ptr) {
	const Counter size = getHeapMemory(ptr);
	decreaseMemory(size);
}

static void allocateMemory(void* ptr, size_t alignment) {
	const Counter size = getHeapMemory(ptr, alignment);
	increaseMemory(size);
}

static void freeMemory(void* ptr, size_t alignment) {
	const Counter size = getHeapMemory(ptr, alignment);
	decreaseMemory(size);
}

#else

Data get() {
	return { 0,0,0,0 };
}

void reset() {}

#endif //GTRACE_MEMORY_BENCH
}


// Override operator new and delete
// Only Windows is supported
#ifdef GTRACE_MEMORY_BENCH
#ifdef _WIN32

void* operator new(std::size_t size) {
	void* res = malloc(size);
	if (res == NULL) throw std::bad_alloc();
	MemoryBench::allocateMemory(res);
	return res;
}

void* operator new[](std::size_t size) {
	void* res = malloc(size);
	if (res == NULL) throw std::bad_alloc();
	MemoryBench::allocateMemory(res);
	return res;
}

void operator delete(void* ptr) {
	if (ptr == nullptr) return;
	MemoryBench::freeMemory(ptr);
	free(ptr);
}

void operator delete[](void* ptr) {
	if (ptr == nullptr) return;
	MemoryBench::freeMemory(ptr);
	free(ptr);
}

void* operator new(std::size_t size, std::align_val_t al) {
	void* res = _aligned_malloc(size, (size_t)al);
	MemoryBench::allocateMemory(res, (size_t)al);
	return res;
}

void operator delete(void* ptr, std::align_val_t al) {
	if (ptr == nullptr) return;
	MemoryBench::freeMemory(ptr, (size_t)al);
	_aligned_free(ptr);
}

#endif // _WIN32
#endif // DEBGTRACE_MEMORY_BENCHUG
}
