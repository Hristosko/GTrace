#include "RendererStat.h"
#include "Logger.h"
#include "scene/World.h"

namespace gtrace {

SecondaryRaysStat::SecondaryRaysStat():
	ligthSamples(0) {}

void RendererStat::renderBegin() {
	this->mainSamples = this->rayCasts = 0;
	this->totalSecondaryRays = SecondaryRaysStat();
}

void RendererStat::updateStat(uint32_t samples, uint32_t rays) {
	std::lock_guard<std::mutex> lock(this->mut);
	this->mainSamples += samples;
	this->rayCasts += rays;
}

void RendererStat::updateStat(const SecondaryRaysStat& sr) {
	std::lock_guard<std::mutex> lock(this->mut);
	this->totalSecondaryRays.ligthSamples += sr.ligthSamples;
}


void RendererStat::renderFinish(const World& w) const {
	const float denom = 1.f / ((float)w.getSettings().width * w.getSettings().height);
	const float samplesPerPixel = ((float)this->mainSamples) * denom;
	const float raysPerPixel = ((float)this->rayCasts) * denom;

	LOGSTAT("Taken samples: ", this->mainSamples);
	LOGSTAT("Avarage samples per pixel: ", samplesPerPixel);
	LOGSTAT("Casted rays: ", this->rayCasts);
	LOGSTAT("Avarage rays per pixel: ", raysPerPixel);

	LOGSTAT("Light sampe rays: ", this->totalSecondaryRays.ligthSamples);
	LOGSTAT("Light sampe rays per pixel: ", (float)this->totalSecondaryRays.ligthSamples * denom);
}
}
