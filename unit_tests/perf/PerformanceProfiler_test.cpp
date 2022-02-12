#include <thread>

#include "TestUtils.h"
#include "Threaded.h"
#include "perf/PerformaceProfiler.h"

using namespace gtrace;

class PerformanceProfilerTest : public Test {
public:
    using TestPerformanceProfiler = PerformanceProfiler<int8_t>;
    using TestProfilerRun = ProfilerRun<int8_t>;

    TestProfilerRun getProfilerRun() {
        auto run = makeProfileRun(&profiler);
        run.enter();
        return run;
    }

    void simulateWork(uint64_t ms) {
        std::this_thread::sleep_for(std::chrono::microseconds(ms));
    }

    void SetUp() override {
        profiler.reset();
    }

    TestPerformanceProfiler profiler;
};

TEST_F(PerformanceProfilerTest, Init) {
    const auto perf = profiler.perf(26);
    EXPECT_EQ(0, perf.count);
    EXPECT_EQ(0, perf.time);

    const auto totalPerf = profiler.perf();
    EXPECT_EQ(0, totalPerf.count);
    EXPECT_EQ(0, totalPerf.time);
}

TEST_F(PerformanceProfilerTest, ProfilerRunDisabled) {
    const auto expectedResult = 26;
    const auto runTime = 1000;
    auto run = makeProfileRun<int8_t>(nullptr);
    run.enter();

    simulateWork(runTime);
    auto res = run.exit(expectedResult);

    EXPECT_EQ(expectedResult, res);
    const auto perf = profiler.perf(res);
    EXPECT_EQ(0, perf.count);
    EXPECT_EQ(0, perf.time);
}

TEST_F(PerformanceProfilerTest, ProfilerRun) {
    const auto expectedResult = 26;
    const auto runTime = 10000;

    auto p = getProfilerRun();
    simulateWork(runTime);
    const auto result = p.exit(expectedResult);

    ASSERT_EQ(expectedResult, result);
    const auto perf = profiler.perf(expectedResult);
    const auto totalPerf = profiler.perf();

    EXPECT_EQ(1, totalPerf.count);
    EXPECT_EQ(1, perf.count);

    EXPECT_GE(totalPerf.time, runTime);
    EXPECT_GE(perf.time, runTime);

    const auto errorTreshold = 1200;
    EXPECT_LT(totalPerf.time, runTime + errorTreshold);
    EXPECT_LT(perf.time, runTime + errorTreshold);
}

TEST_F(PerformanceProfilerTest, MultiThreaded) {
    const auto result = 26;
    const auto runTime = 10000;

    struct TestThreaded : Threaded {
        TestThreaded (PerformanceProfilerTest* test, int8_t result, uint64_t runTime) :
            test(test),
            result(result),
            runTime(runTime) {}
        PerformanceProfilerTest* test;
        const int8_t result;
        const uint64_t runTime;

        void run(unsigned threadIdx, unsigned numThreads) override {
            auto p = test->getProfilerRun();
            test->simulateWork(runTime);
            p.exit(result);
        }
    };

    TestThreaded th(this, result, runTime);
    const auto numThreads = 2 * std::thread::hardware_concurrency();
    ThreadManager tm(numThreads);
    tm.run(&th);

    const auto perf = profiler.perf(result);
    EXPECT_EQ(numThreads, perf.count);
    EXPECT_GE(perf.time, runTime * numThreads);
}

class PerformanceProfilerVoidTest : public Test {
public:
    using TestPerformanceProfiler = PerformanceProfiler<void>;
    using TestProfilerRun = ProfilerRun<void>;

    TestProfilerRun getProfilerRun() {
        auto run = makeProfileRun(&profiler);
        run.enter();
        return run;
    }

    void simulateWork(uint64_t ms) {
        std::this_thread::sleep_for(std::chrono::microseconds(ms));
    }

    void SetUp() override {
        profiler.reset();
    }

    TestPerformanceProfiler profiler;
};

TEST_F(PerformanceProfilerVoidTest, Init) {
    const auto totalPerf = profiler.perf();
    EXPECT_EQ(0, totalPerf.count);
    EXPECT_EQ(0, totalPerf.time);
}

TEST_F(PerformanceProfilerVoidTest, ProfilerRunDisabled) {
    const auto runTime = 1000;
    auto run = makeProfileRun<void>(nullptr);
    run.enter();

    simulateWork(runTime);
    run.exit();

    const auto perf = profiler.perf();
    EXPECT_EQ(0, perf.count);
    EXPECT_EQ(0, perf.time);
}

TEST_F(PerformanceProfilerVoidTest, ProfilerRun) {
    const auto runTime = 10000;

    auto p = getProfilerRun();
    simulateWork(runTime);
    p.exit();

    const auto perf = profiler.perf();

    EXPECT_EQ(1, perf.count);
    EXPECT_GE(perf.time, runTime);
}
