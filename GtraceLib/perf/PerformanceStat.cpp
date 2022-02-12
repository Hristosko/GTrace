#include "PerformanceStat.h"

namespace gtrace {

Performance& Performance::operator+=(const Performance& rhs) {
    count += rhs.count;
    time += rhs.time;
    average = static_cast<float>(time) / count;
    return *this;
}

std::string Performance::toString() const {
    const std::string res =
    "\ncount: " + std::to_string(count) +
    "\nime: " + std::to_string(time) +
    "\naverage: " + std::to_string(average);
    return res;
}

void PerformanceStat::add(uint64_t count, uint64_t time) {
    this->count.fetch_add(count, std::memory_order::memory_order_relaxed);
    this->time.fetch_add(time, std::memory_order::memory_order_relaxed);
}

Performance PerformanceStat::perf() const {
    Performance p;
    p.count = count.load(std::memory_order::memory_order_relaxed);
    p.time = time.load(std::memory_order::memory_order_relaxed);
    p.average = static_cast<float>(p.time) / p.count;

    return p;
}

void PerformanceStat::reset() {
    count.store(0, std::memory_order::memory_order_relaxed);
    time.store(0, std::memory_order::memory_order_relaxed);
}

}
