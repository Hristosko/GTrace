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
#ifdef __unix__
    Counter totalRequestedMemory;
#endif // __unix__
};

Data get();
void reset();
void logData();

};
}
