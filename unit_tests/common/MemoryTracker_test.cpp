#include "TestUtils.h"
#include "common/MemoryTracker.h"
#include "common/Threaded.h"

using namespace gtrace;
class MemoryTrackerTest : public Test
{
public:
    void SetUp() override { MemoryTracker::reset(); }

    static void allocateMemory(int** res, int count)
    {
        for (int i = 0; i < count; ++i)
            res[i] = new int;
    }

    static void freeMemory(int** ptrs, int count)
    {
        for (int i = 0; i < count; ++i)
            delete ptrs[i];
    }
};

struct ThreadedAllocation : Threaded
{
    ThreadedAllocation(int** res, int count) : res(res), count(count) {}

    void run(unsigned threadIdx, unsigned numThreads) override
    {
        MemoryTrackerTest::allocateMemory(res + threadIdx * count, count);
    }

    int** res;
    int count;
};

struct ThreadedFree : Threaded
{
    ThreadedFree(int** ptrs, int count) : ptrs(ptrs), count(count) {}

    void run(unsigned threadIdx, unsigned numThreads) override
    {
        MemoryTrackerTest::freeMemory(ptrs + threadIdx * count, count);
    }

    int** ptrs;
    int count;
};

TEST_F(MemoryTrackerTest, reset)
{
    const auto stats = MemoryTracker::getStatistic();
    ASSERT_EQ(0, stats.allocationCount);
    ASSERT_EQ(0, stats.freeCount);
    ASSERT_EQ(0, stats.peakMemoryUsage);
    ASSERT_EQ(0, stats.totalAllocatedMemory);
    ASSERT_EQ(0, stats.totalRequestedMemory);
}

TEST_F(MemoryTrackerTest, singleAlocationAndFree)
{
    auto* ptr = new int;
    auto stats = MemoryTracker::getStatistic();

    ASSERT_EQ(1, stats.allocationCount);
    ASSERT_EQ(0, stats.freeCount);
    ASSERT_LE(sizeof(int), stats.peakMemoryUsage);
    ASSERT_LE(sizeof(int), stats.totalAllocatedMemory);
#ifdef __unix__
    ASSERT_EQ(sizeof(int), stats.totalRequestedMemory);
#endif  // __unix__

    delete ptr;

    stats = MemoryTracker::getStatistic();

    ASSERT_EQ(1, stats.allocationCount);
    ASSERT_EQ(1, stats.freeCount);
    ASSERT_LE(sizeof(int), stats.peakMemoryUsage);
    ASSERT_LE(sizeof(int), stats.totalAllocatedMemory);
#ifdef __unix__
    ASSERT_EQ(sizeof(int), stats.totalRequestedMemory);
#endif  // __unix__
}

TEST_F(MemoryTrackerTest, singleAlocationAndFreeArray)
{
    constexpr auto N = 10;
    auto* ptr = new int[N];
    auto stats = MemoryTracker::getStatistic();

    ASSERT_EQ(1, stats.allocationCount);
    ASSERT_EQ(0, stats.freeCount);
    ASSERT_LE(N * sizeof(int), stats.peakMemoryUsage);
    ASSERT_LE(N * sizeof(int), stats.totalAllocatedMemory);
#ifdef __unix__
    ASSERT_EQ(N * sizeof(int), stats.totalRequestedMemory);
#endif  // __unix__

    delete[] ptr;

    stats = MemoryTracker::getStatistic();

    ASSERT_EQ(1, stats.allocationCount);
    ASSERT_EQ(1, stats.freeCount);
    ASSERT_LE(N * sizeof(int), stats.peakMemoryUsage);
    ASSERT_LE(N * sizeof(int), stats.totalAllocatedMemory);
#ifdef __unix__
    ASSERT_EQ(N * sizeof(int), stats.totalRequestedMemory);
#endif  // __unix__
}

TEST_F(MemoryTrackerTest, multipleAllocationsSingleThread)
{
    constexpr auto N = 10;
    int* ptrs[N];
    allocateMemory(ptrs, N);
    auto stats = MemoryTracker::getStatistic();

    ASSERT_EQ(N, stats.allocationCount);
    ASSERT_EQ(0, stats.freeCount);
    ASSERT_LE(N * sizeof(int), stats.peakMemoryUsage);
    ASSERT_LE(N * sizeof(int), stats.totalAllocatedMemory);
#ifdef __unix__
    ASSERT_EQ(N * sizeof(int), stats.totalRequestedMemory);
#endif  // __unix__

    freeMemory(ptrs, N);
    stats = MemoryTracker::getStatistic();

    ASSERT_EQ(N, stats.allocationCount);
    ASSERT_EQ(N, stats.freeCount);
    ASSERT_LE(N * sizeof(int), stats.peakMemoryUsage);
    ASSERT_LE(N * sizeof(int), stats.totalAllocatedMemory);
#ifdef __unix__
    ASSERT_EQ(N * sizeof(int), stats.totalRequestedMemory);
#endif  // __unix__
}

TEST_F(MemoryTrackerTest, peakMemoryUsage)
{
    constexpr auto N = 10;
    int* ptrs[2][N];
    allocateMemory(ptrs[0], N);
    allocateMemory(ptrs[1], N);
    freeMemory(ptrs[1], N);
    allocateMemory(ptrs[1], N);
    freeMemory(ptrs[1], N);
    freeMemory(ptrs[0], N);

    auto stats = MemoryTracker::getStatistic();

    ASSERT_EQ(3 * N, stats.allocationCount);
    ASSERT_EQ(3 * N, stats.freeCount);
    ASSERT_LE(2 * N * sizeof(int), stats.peakMemoryUsage);
    ASSERT_LE(3 * N * sizeof(int), stats.totalAllocatedMemory);
#ifdef __unix__
    ASSERT_EQ(3 * N * sizeof(int), stats.totalRequestedMemory);
#endif  // __unix__
}

TEST_F(MemoryTrackerTest, ThreadedAllocationAndFree)
{
    constexpr auto N = 30;
    constexpr auto numThreads = 8;
    int* ptrs[numThreads * N];
    ThreadedAllocation allocator(ptrs, N);
    ThreadedFree deleter(ptrs, N);

    ThreadManager manager(numThreads);
    manager.run(&allocator);
    manager.run(&deleter);

    auto stats = MemoryTracker::getStatistic();

    constexpr auto aditionalAllocation = 2 * (1 + numThreads);
    ASSERT_EQ(numThreads * N + aditionalAllocation, stats.allocationCount);
    ASSERT_EQ(numThreads * N + aditionalAllocation, stats.freeCount);
    ASSERT_LE(numThreads * N * sizeof(int), stats.peakMemoryUsage);
    ASSERT_LE(numThreads * N * sizeof(int), stats.totalAllocatedMemory);
}
