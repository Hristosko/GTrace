#pragma once
#include <atomic>
#include <string>

namespace gtrace {

struct Performance {
    uint64_t count = 0;
    uint64_t time = 0;
    float average = 0.f;

    Performance& operator+=(const Performance& rhs);
    std::string toString() const;
};

class PerformanceStat {
public:
    void add(uint64_t count, uint64_t time);
    Performance perf() const;

    void reset();
private:
    std::atomic<uint64_t> count;
    std::atomic<uint64_t> time;
};

}
