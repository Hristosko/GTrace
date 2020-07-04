#include "RendererOutput.h"
#include "../scene/World.h"

void RendererOutput::init() {
	this->initImageOutput();
}

void RendererOutput::initImageOutput() {
	const SceneSettings& settings = getWorld().getSettings();
	DataBuffer& buffer = this->getOutput(RendererOutputType::Image);
	buffer.init<ColorResult>((uint64_t)settings.width * settings.height);
}
