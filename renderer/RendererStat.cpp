#include "RendererStat.h"
#include "../Logger.h"
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
	LOGINFO("Taken samples: ", this->mainSamples,
		" Avarage samples per pixel: ", mainSamples,
		" Casted rays: ", this->rayCasts,
		" Avarage rays per pixel: ", raysPerPixel);
}
