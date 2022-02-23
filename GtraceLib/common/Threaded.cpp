#include <memory>
#include <thread>
#include "Threaded.h"

namespace gtrace
{
void ThreadManager::run(Threaded* th) const
{
    auto threads = std::make_unique<std::thread[]>(numThreads);

    for (unsigned idx = 0; idx < numThreads; ++idx)
        threads[idx] = std::thread(&ThreadManager::threadFunc, this, idx, th);

    for (unsigned idx = 0; idx < numThreads; ++idx)
        threads[idx].join();
}

void ThreadManager::threadFunc(unsigned threadIdx, Threaded* th) const
{
    th->run(threadIdx, numThreads);
}
}  // namespace gtrace