#pragma once

#include <thread>
#include <vector>
#include "Logger.h"

namespace gtrace {

class Threaded {
public:
	virtual void run(unsigned threadIdx, unsigned numThreads) = 0;
};

class ThreadManager {
public:
	ThreadManager(unsigned numThreads)
		: numThreads(numThreads) {}

	void run(Threaded* th) {
		this->threads.reserve(numThreads);

		for (unsigned idx = 0; idx < numThreads; ++idx) {
			this->threads.push_back(std::thread(&ThreadManager::threadFunc, this, idx, th));
		}
		for (std::thread& curth : threads) curth.join();
	}
private:
	void threadFunc(unsigned threadIdx, Threaded * th) {
		LOGINFO("Thread start: ", threadIdx, "/", this->numThreads);
		th->run(threadIdx, numThreads);
		LOGINFO("Thread finish: ", threadIdx, "/", this->numThreads);
	}
private:
	std::vector<std::thread> threads;
	unsigned numThreads;
};
}
