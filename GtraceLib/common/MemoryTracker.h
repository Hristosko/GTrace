#pragma once
#include <stdint.h>

namespace gtrace
{
class MemoryTracker
{
public:
    using Counter = uint64_t;

    struct Stats
    {
        Counter totalAllocatedMemory;
        Counter peakMemoryUsage;
        Counter allocationCount;
        Counter freeCount;
        Counter totalRequestedMemory;
    };

    static void reset();
    static Stats getStatistic();
    static void logStatistic();
};
}  // namespace gtrace