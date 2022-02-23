#pragma once

namespace gtrace
{
class Threaded
{
public:
    virtual void run(unsigned threadIdx, unsigned numThreads) = 0;
};

class ThreadManager
{
public:
    ThreadManager(unsigned numThreads) : numThreads(numThreads) {}

    void run(Threaded* th) const;

private:
    void threadFunc(unsigned threadIdx, Threaded* th) const;

private:
    unsigned numThreads;
};
}  // namespace gtrace
