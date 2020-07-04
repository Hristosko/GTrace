#include "RendererOutput.h"

RendererOutput::RendererOutput(uint32_t width, uint32_t height)
	: width(width), height(height)
{
	this->initImageOutput();
}

void RendererOutput::initImageOutput() {
	DataBuffer& buffer = this->getOutput(RendererOutputType::Image);
	buffer.init<ColorResult>((uint64_t)this->width * this->height);
	
	char* pixelData = reinterpret_cast<char*>(buffer.getBuffer());
	for (int y = 0; y < this->height; ++y)
	{
		for (int x = 0; x < this->width; ++x)
		{
			pixelData[3 * y * width + 3 * x] =  255;
			pixelData[3 * y * width + 3 * x + 1] = 0;
			pixelData[3 * y * width + 3 * x + 2] = 0;
		}
	}
}
