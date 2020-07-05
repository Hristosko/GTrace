#include "RendererOutput.h"
#include "../scene/World.h"

void RendererOutput::init() {
	this->initImageOutput();
	this->initVarianceOutput();
}

void RendererOutput::initImageOutput() {
	const SceneSettings& settings = getWorld().getSettings();
	DataBuffer& buffer = this->getOutput(RendererOutputType::Image);
	buffer.init<ColorResult>((uint64_t)settings.width * settings.height);
}

void RendererOutput::initVarianceOutput() {
	const SceneSettings& settings = getWorld().getSettings();
	DataBuffer& buffer = this->getOutput(RendererOutputType::Variance);
	buffer.init<VarianceResult>((uint64_t)settings.width * settings.height);
}
