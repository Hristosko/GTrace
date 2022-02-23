#include <atomic>
#include "MemoryTracker.h"
#include "Logger.h"

namespace gtrace
{
using Counter = MemoryTracker::Counter;

struct InteralMemoryStats
{
    std::atomic<Counter> currentAllocatedMemory;
    std::atomic<Counter> peakAllocatedMemory;
    std::atomic<Counter> totalAllocatedMemory;
    std::atomic<Counter> allocationCount;
    std::atomic<Counter> freeCount;
    std::atomic<Counter> totalRequestedMemory;

    InteralMemoryStats() :
        currentAllocatedMemory(0),
        peakAllocatedMemory(0),
        totalAllocatedMemory(0),
        allocationCount(0),
        freeCount(0),
        totalRequestedMemory(0)
    {
    }
};

static InteralMemoryStats memoryStat;

void MemoryTracker::reset()
{
    memoryStat.currentAllocatedMemory.store(0, std::memory_order_relaxed);
    memoryStat.peakAllocatedMemory.store(0, std::memory_order_relaxed);
    memoryStat.totalAllocatedMemory.store(0, std::memory_order_relaxed);
    memoryStat.allocationCount.store(0, std::memory_order_relaxed);
    memoryStat.freeCount.store(0, std::memory_order_relaxed);
    memoryStat.totalRequestedMemory.store(0, std::memory_order_relaxed);
}

MemoryTracker::Stats MemoryTracker::getStatistic()
{
    return {memoryStat.totalAllocatedMemory.load(std::memory_order::memory_order_relaxed),
            memoryStat.peakAllocatedMemory.load(std::memory_order::memory_order_relaxed),
            memoryStat.allocationCount.load(std::memory_order::memory_order_relaxed),
            memoryStat.freeCount.load(std::memory_order::memory_order_relaxed),
            memoryStat.totalRequestedMemory.load(std::memory_order::memory_order_relaxed)};
}

void MemoryTracker::logStatistic()
{
    auto stats = getStatistic();
    const auto bToMb = [](int x) { return static_cast<float>(x) / (1024 * 1024); };
#ifdef __unix__
    LOGSTAT("Total requested memory: ", stats.totalRequestedMemory, "B ", bToMb(stats.totalRequestedMemory), "MB");
#endif  // __unix__
    LOGSTAT("Total alocated memory: ", stats.totalAllocatedMemory, "B ", bToMb(stats.totalAllocatedMemory), "MB");
    LOGSTAT("Peak memory usage: ", stats.peakMemoryUsage, "B ", bToMb(stats.peakMemoryUsage), "MB");
    LOGSTAT("Alocations count: ", stats.allocationCount);
    LOGSTAT("Freed allocations count: ", stats.freeCount);
}

/**
 * Return the size of the allocated memory.
 * For Windows this will return the requested memory,
 * for Linux - the actual size on the heap
 */
static Counter getHeapMemory(void* ptr)
{
#ifdef _WIN32
    return _msize(ptr);
#endif  // _WIN32
#ifdef __unix__
    return *(reinterpret_cast<Counter*>(ptr) - 1);
#endif  // __unix__
}

/**
 * Same as getHeapMemory(void* ptr) but for aligned allocation,
 * see HeapAligned.h
 */
#ifdef _WIN32
static Counter getHeapMemory(void* ptr, size_t alignment)
{
    return _aligned_msize(ptr, alignment, 0 /* the whole struct is alligned like this */);
}
#endif  // _WIN32

/**
 * Accumulates single allocation with the given size.
 */
static void increaseMemory(Counter size)
{
    memoryStat.allocationCount.fetch_add(1, std::memory_order_relaxed);
    memoryStat.currentAllocatedMemory.fetch_add(size, std::memory_order_relaxed);

    Counter peak = memoryStat.peakAllocatedMemory.load(std::memory_order_relaxed);
    Counter cur = memoryStat.currentAllocatedMemory.load(std::memory_order_relaxed);

    while (peak < cur && memoryStat.peakAllocatedMemory.compare_exchange_weak(
                             peak, cur, std::memory_order_relaxed, std::memory_order_relaxed))
    {
    }

    memoryStat.totalAllocatedMemory.fetch_add(size, std::memory_order_relaxed);
}

/**
 * Accumulates single mem. free with the given size.
 */
static void decreaseMemory(Counter size)
{
    memoryStat.freeCount.fetch_add(1, std::memory_order_relaxed);
    memoryStat.currentAllocatedMemory.fetch_sub(size, std::memory_order_relaxed);
}

static void allocateMemory(void* ptr)
{
    const Counter size = getHeapMemory(ptr);
    increaseMemory(size);
}

static void freeMemory(void* ptr)
{
    const Counter size = getHeapMemory(ptr);
    decreaseMemory(size);
}

#ifdef __unix__
static void requestMemory(Counter size)
{
    memoryStat.totalRequestedMemory.fetch_add(size, std::memory_order_relaxed);
}
#endif  //__unix__

#ifdef _WIN32
static void allocateMemory(void* ptr, size_t alignment)
{
    const Counter size = getHeapMemory(ptr, alignment);
    increaseMemory(size);
}

static void freeMemory(void* ptr, size_t alignment)
{
    const Counter size = getHeapMemory(ptr, alignment);
    decreaseMemory(size);
}
#endif  //_WIN32

}  // namespace gtrace

// Override operator new and delete
#ifdef GTRACE_MEMORY_TRACKING

void* operator new(std::size_t size)
{
    void* res = malloc(size);
    if (res == NULL)
        throw std::bad_alloc();
    gtrace::allocateMemory(res);
#ifdef __unix__
    gtrace::requestMemory(size);
#endif  //__unix
    return res;
}

void* operator new[](std::size_t size)
{
    void* res = malloc(size);
    if (res == NULL)
        throw std::bad_alloc();
    gtrace::allocateMemory(res);
#ifdef __unix__
    gtrace::requestMemory(size);
#endif  //__unix
    return res;
}

void operator delete(void* ptr)
{
    if (ptr == nullptr)
        return;
    gtrace::freeMemory(ptr);
    free(ptr);
}

void operator delete[](void* ptr)
{
    if (ptr == nullptr)
        return;
    gtrace::freeMemory(ptr);
    free(ptr);
}

#ifdef _WIN32
void* operator new(std::size_t size, std::align_val_t al)
{
    void* res = _aligned_malloc(size, (size_t)al);
    gtrace::allocateMemory(res, (size_t)al);
    return res;
}

void operator delete(void* ptr, std::align_val_t al)
{
    if (ptr == nullptr)
        return;
    gtrace::freeMemory(ptr, (size_t)al);
    _aligned_free(ptr);
}
#endif  // _WIN32

#endif  // DEBGTRACE_MEMORY_TRACKING
