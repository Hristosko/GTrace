#pragma once

#include <cstdint>
#include "Logger.h"

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

struct Scope {
	Scope() { reset(); }
	~Scope() {
		const Data mb = get();
		LOGSTAT("Total alocated memory: ", mb.totalAllocatedMemory, "B ", mb.totalAllocatedMemory / (1024 * 1024), "MB");
		LOGSTAT("Peak memory usage: ", mb.peakMemoryUsage, "B ", mb.peakMemoryUsage / (1024 * 1024), "MB");
		LOGSTAT("Alocations count: ", mb.allocationCount);
		LOGSTAT("Freed allocations count: ", mb.freeCount);
	}
};

};
