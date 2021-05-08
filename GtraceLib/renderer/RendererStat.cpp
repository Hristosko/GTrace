#include "RendererStat.h"
#include "Logger.h"
#include "scene/World.h"

namespace gtrace {

void RendererStat::renderBegin() {
	this->mainSamples = this->rayCasts = 0;
}

void RendererStat::updateStat(uint32_t samples, uint32_t rays) {
	this->mut.lock();
	this->mainSamples += samples;
	this->rayCasts += rays;
	this->mut.unlock();
}

void RendererStat::renderFinish(const World& w) const {
	const float denom = (float)w.getSettings().width * w.getSettings().height;
	const float samplesPerPixel = ((float)this->mainSamples) / denom;
	const float raysPerPixel = ((float)this->rayCasts) / denom;
	LOGSTAT("Taken samples: ", this->mainSamples);
	LOGSTAT("Avarage samples per pixel: ", samplesPerPixel);
	LOGSTAT("Casted rays: ", this->rayCasts);
	LOGSTAT("Avarage rays per pixel: ", raysPerPixel);
}
}
