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

Data get();

};
