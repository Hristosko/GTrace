#pragma once


#include <cstdint>

namespace MemoryBench {

using Counter = uint64_t;
struct Data {
	Counter totalAllocatedMemory;
	Counter peakMemoryUsage;
	Counter allocationCount;
	Counter freeCount;
};

void init();
Data get();

void allocate(void* ptr);
void free(void* ptr);

void allocate(void* ptr, size_t alignment);
void free(void* ptr, size_t alignment);

};
