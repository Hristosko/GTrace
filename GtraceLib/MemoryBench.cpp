#include "MemoryBench.h"
#include "Logger.h"

#include <algorithm> // std:::max
#include <atomic>

namespace gtrace {

namespace MemoryBench {

#ifdef GTRACE_MEMORY_BENCH


struct InternalData {
	std::atomic<Counter> currentAllocatedMemory;
	std::atomic<Counter> peakAllocatedMemory;
	std::atomic<Counter> totalAllocatedMemory;
	std::atomic<Counter> allocationCount;
	std::atomic<Counter> freeCount;
#ifdef __unix__
	std::atomic<Counter> totalRequestedMemory;
#endif

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
 * for Linux - the actual size on the heap
 */
static Counter getHeapMemory(void* ptr) {
#ifdef _WIN32
	return _msize(ptr);
#endif // _WIN32
#ifdef __unix__
	return *(reinterpret_cast<Counter*>(ptr) - 1);
#endif // __unix__
}

/**
 * Same as getHeapMemory(void* ptr) but for aligned allocation,
 * see HeapAligned.h
 */
 #ifdef _WIN32
static Counter getHeapMemory(void* ptr, size_t alignment) {
	return _aligned_msize(ptr, alignment, 0 /* the whole struct is alligned like this */);
}
#endif // _WIN32

/**
 * Get the currently accumulated data.
 */
Data get() {
	return {
		id.totalAllocatedMemory.load(std::memory_order::memory_order_relaxed),
		id.peakAllocatedMemory.load(std::memory_order::memory_order_relaxed),
		id.allocationCount.load(std::memory_order::memory_order_relaxed),
		id.freeCount.load(std::memory_order::memory_order_relaxed)
#ifdef __unix__
		,id.totalRequestedMemory.load(std::memory_order::memory_order_relaxed)
#endif //__unix__
};
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

void logData() {
	const Data mb = get();
#ifdef __unix__
	LOGSTAT("Total requested memory: ", mb.totalRequestedMemory, "B ", (float)mb.totalRequestedMemory / (1024 * 1024), "MB");
#endif // __unix__
	LOGSTAT("Total alocated memory: ", mb.totalAllocatedMemory, "B ", (float)mb.totalAllocatedMemory / (1024 * 1024), "MB");
	LOGSTAT("Peak memory usage: ", mb.peakMemoryUsage, "B ", (float)mb.peakMemoryUsage / (1024 * 1024), "MB");
	LOGSTAT("Alocations count: ", mb.allocationCount);
	LOGSTAT("Freed allocations count: ", mb.freeCount);
}

/**
 * Accumulates single allocation with the given size.
 */
static void increaseMemory(Counter size) {
	id.allocationCount.fetch_add(1, std::memory_order_relaxed);
	id.currentAllocatedMemory.fetch_add(size, std::memory_order_relaxed);

	Counter peak = id.peakAllocatedMemory.load(std::memory_order_relaxed);
	Counter cur = id.currentAllocatedMemory.load(std::memory_order_relaxed);

	while (peak < cur &&
		id.peakAllocatedMemory.compare_exchange_weak(peak, cur, std::memory_order_relaxed, std::memory_order_relaxed))
	{}

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

#ifdef __unix__
static void requestMemory(Counter size) {
	id.totalRequestedMemory.fetch_add(size, std::memory_order_relaxed);
}
#endif //__unix__

#ifdef _WIN32
static void allocateMemory(void* ptr, size_t alignment) {
	const Counter size = getHeapMemory(ptr, alignment);
	increaseMemory(size);
}

static void freeMemory(void* ptr, size_t alignment) {
	const Counter size = getHeapMemory(ptr, alignment);
	decreaseMemory(size);
}
#endif //_WIN32

#else

Data get() {
	return { 0,0,0,0 };
}

void reset() {}

void logData();

#endif //GTRACE_MEMORY_BENCH
}

}

// Override operator new and delete
#ifdef GTRACE_MEMORY_BENCH

void* operator new(std::size_t size) {
	void* res = malloc(size);
	if (res == NULL) throw std::bad_alloc();
	gtrace::MemoryBench::allocateMemory(res);
#ifdef __unix__
	gtrace::MemoryBench::requestMemory(size);
#endif //__unix
	return res;
}

void* operator new[](std::size_t size) {
	void* res = malloc(size);
	if (res == NULL) throw std::bad_alloc();
	gtrace::MemoryBench::allocateMemory(res);
#ifdef __unix__
	gtrace::MemoryBench::requestMemory(size);
#endif //__unix
	return res;
}

void operator delete(void* ptr) {
	if (ptr == nullptr) return;
	gtrace::MemoryBench::freeMemory(ptr);
	free(ptr);
}

void operator delete[](void* ptr) {
	if (ptr == nullptr) return;
	gtrace::MemoryBench::freeMemory(ptr);
	free(ptr);
}

#ifdef _WIN32
void* operator new(std::size_t size, std::align_val_t al) {
	void* res = _aligned_malloc(size, (size_t)al);
	gtrace::MemoryBench::allocateMemory(res, (size_t)al);
	return res;
}

void operator delete(void* ptr, std::align_val_t al) {
	if (ptr == nullptr) return;
	gtrace::MemoryBench::freeMemory(ptr, (size_t)al);
	_aligned_free(ptr);
}
#endif // _WIN32

#endif // DEBGTRACE_MEMORY_BENCHUG
