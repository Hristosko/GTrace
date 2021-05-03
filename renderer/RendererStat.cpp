#include "RendererStat.h"
#include "../Logger.h"
#include "../MemoryBench.h"
#include "../scene/World.h"

void RendererStat::renderBegin() {
	this->mainSamples = this->rayCasts = 0;
}

void RendererStat::updateStat(uint32_t samples, uint32_t rays) {
	this->mut.lock();
	this->mainSamples += samples;
	this->rayCasts += rays;
	this->mut.unlock();
}

void RendererStat::renderFinish() const {
	const float denom = (float)getWorld().getSettings().width * getWorld().getSettings().height;
	const float samplesPerPixel = ((float)this->mainSamples) / denom;
	const float raysPerPixel = ((float)this->rayCasts) / denom;
	const MemoryBench::Data mb = MemoryBench::get();
	LOGINFO(
		"\nTaken samples: ", this->mainSamples,
		"\nAvarage samples per pixel: ", samplesPerPixel,
		"\nCasted rays: ", this->rayCasts,
		"\nAvarage rays per pixel: ", raysPerPixel,
		"\nTotal alocated memory: ", mb.totalAllocatedMemory,
		"\nPeak memory usage: ", mb.peakMemoryUsage,
		"\nAlocations count: ", mb.allocationCount,
		"\nFreed allocations count: ", mb.freeCount);
}
