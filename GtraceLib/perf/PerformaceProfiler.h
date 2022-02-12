#pragma once

#include <array>
#include <limits>
#include <chrono>

#include "PerformanceStat.h"

namespace gtrace {

template<typename T>
std::string asString(T x) { return std::to_string(x); }

template<>
std::string asString<bool>(bool x) {
    return x ? "True" : "False";
}

class PerformanceProfilerBase {
public:
    PerformanceProfilerBase(const char* name) : name(name) {}

    const std::string& getName() const { return name; }
protected:
    static constexpr const char* defaultName = "DefaultPerformanceProfiler";
    std::string name;
};

template<typename ReturnType>
class PerformanceProfiler : PerformanceProfilerBase {
public:
    using Type = ReturnType;
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    PerformanceProfiler(const char* name = defaultName) : PerformanceProfilerBase(name) {}

    void update(Type value, const TimePoint& start, const TimePoint& end) {
        const auto time = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
        auto& stat = stats[getIndex(value)];
        stat.add(1, time);
    }

    Performance perf(Type value) const {
        const auto& stat = stats[getIndex(value)];
        return stat.perf();
    }

    Performance perf() const {
        Performance p;

        for (Type i = std::numeric_limits<Type>::min();; ++i)
        {
            p += perf(i);
            if (i == std::numeric_limits<Type>::max()) break;
        }
        return p;
    }

    void reset() {
        for (auto& stat : stats) {
            stat.reset();
        }
    }

    std::string toString() const {
        std::string res = name;

        res += '\n';
        res += "Total: {" + perf().toString() + "}\n";
        for (Type i = std::numeric_limits<Type>::min();; ++i)
        {
            res += asString<Type>(i) + ": {" + perf(i).toString() + "}\n";
            if (i == std::numeric_limits<Type>::max()) break;
        }

        return res;
    }

private:
    static constexpr size_t getIndex(Type x) {
        return static_cast<size_t>(x) - std::numeric_limits<Type>::min();
    }

    static constexpr size_t size() {
        return getIndex(std::numeric_limits<Type>::max()) + 1;
    }

    std::string name;
    std::array<PerformanceStat, size()> stats;
};

template<typename Profiler>
class ScopeProfiler {
public:
    ScopeProfiler(Profiler& p) : profiler(p), start(Profiler::Clock::now()) {}

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