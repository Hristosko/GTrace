#include "RendererOutput.h"

RendererOutput::RendererOutput(uint32_t width, uint32_t height)
	: width(width), height(height)
{
	this->initImageOutput();
}

void RendererOutput::initImageOutput() {
	DataBuffer& buffer = this->getOutput(RendererOutputType::Image);
	buffer.init<ColorResult>((uint64_t)this->width * this->height);
}
