#pragma once

#include <array>
#include <atomic>
#include <limits>
#include <chrono>

namespace gtrace {

struct Performance {
    uint64_t count;
    uint64_t time;

    Performance& operator+=(const Performance& rhs) {
        count += rhs.count;
        time += rhs.time;
        return *this;
    }
};

template<typename ReturnType>
class PerformanceProfiler {
public:
    using Type = ReturnType;
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    void update(Type value, const TimePoint& start, const TimePoint& end) {
        const auto time = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
        auto& stat = stats[value];
        stat.count.fetch_add(1, std::memory_order::memory_order_relaxed);
        stat.time.fetch_add(time, std::memory_order::memory_order_relaxed);
    }

    Performance perf(Type value) const {
        const auto& stat = stats[value];
        return Performance{
            stat.count.load(std::memory_order::memory_order_relaxed),
            stat.time.load(std::memory_order::memory_order_relaxed)
        };
    }

    Performance perf() const {
        Performance p {0, 0};

        for (size_t i = 0; i < stats.size(); ++i)
            p += perf(i);

        return p;
    }

    void reset() {
        for (auto& stat : stats) {
            stat.count.store(0, std::memory_order::memory_order_relaxed);
            stat.time.store(0, std::memory_order::memory_order_relaxed);
        }
    }

private:
    struct PerfStat {
        std::atomic<uint64_t> count;
        std::atomic<uint64_t> time;
    };

    static constexpr size_t size() {
        return std::numeric_limits<Type>::max() - std::numeric_limits<Type>::min();
    }

    std::array<PerfStat, std::numeric_limits<Type>::max()> stats;
};

template<typename Profiler>
class FunctionProfiler {
public:
    FunctionProfiler(Profiler& p) : profiler(p), start(Profiler::Clock::now()) {}

    typename Profiler::Type exit(typename Profiler::Type value) {
        typename Profiler::TimePoint end = Profiler::Clock::now();
        profiler.update(value, start, end);
        return value;
    }
private:
    Profiler& profiler;
    typename Profiler::TimePoint start;
};

}