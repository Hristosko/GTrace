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
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;
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

    std::array<PerformanceStat, size()> stats;
};

template<>
class PerformanceProfiler<void> : PerformanceProfilerBase {
public:
    PerformanceProfiler(const char* name = defaultName) : PerformanceProfilerBase(name) {}

    void update(const TimePoint& start, const TimePoint& end) {
        const auto time = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
        stat.add(1, time);
    }

    Performance perf() const {
        return stat.perf();
    }

    std::string toString() const {
        return name + perf().toString();
    }

    void reset() {
        stat.reset();
    }

private:
    PerformanceStat stat;
};

template<typename T>
struct ProfilerRunBase {
    using EnterHandler = void(*)(ProfilerRunBase& run);

    static void enterEnabled(ProfilerRunBase& run) {
        run.start = PerformanceProfilerBase::Clock::now();
    }
    static void enterDisabled(ProfilerRunBase& run) {}

    EnterHandler onEnter;
    PerformanceProfilerBase::TimePoint start;
    PerformanceProfiler<T>* profiler;
};

template<typename T>
struct ProfilerRun : public ProfilerRunBase<T> {
    using ExitHandler = void(*)(ProfilerRun& run, T value);

    static void exitEnabled(ProfilerRun& run, T value) {
        PerformanceProfilerBase::TimePoint end = PerformanceProfilerBase::Clock::now();
        run.profiler->update(value, run.start, end);
    }
    static void exitDisabled(ProfilerRun& run, T value) {}

    void enter() { ProfilerRunBase<T>::onEnter(*this); }
    T exit(T value) {
        onExit(*this, value);
        return value;
    }

    ExitHandler onExit;
};

template<>
struct ProfilerRun<void> : public ProfilerRunBase<void> {
    using ExitHandler = void(*)(ProfilerRun& run);

    static void exitEnabled(ProfilerRun& run) {
        PerformanceProfilerBase::TimePoint end = PerformanceProfilerBase::Clock::now();
        run.profiler->update(run.start, end);
    }
    static void exitDisabled(ProfilerRun& run) {}

    void enter() { ProfilerRunBase<void>::onEnter(*this); }
    void exit() { onExit(*this); }

    ExitHandler onExit;
};

template<typename T>
ProfilerRun<T> makeProfileRun(PerformanceProfiler<T>* profiler) {
    ProfilerRun<T> run;
    run.profiler = profiler;
    if (profiler) {
        run.onEnter = ProfilerRun<T>::enterEnabled;
        run.onExit = ProfilerRun<T>::exitEnabled;
    }
    else {
        run.onEnter = ProfilerRun<T>::enterDisabled;
        run.onExit = ProfilerRun<T>::exitDisabled;
    }
    return run;
}

}