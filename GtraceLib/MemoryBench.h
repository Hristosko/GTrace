#pragma once

#include <cstdint>
#include "Logger.h"

namespace gtrace {

namespace MemoryBench {

using Counter = uint64_t;
struct Data {
	Counter totalAllocatedMemory;
	Counter peakMemoryUsage;
	Counter allocationCount;
	Counter freeCount;
};

Data get();
void reset();

};
}
