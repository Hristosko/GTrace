#include "RendererDisplay.h"
#include "../scene/World.h"

RendererDisplay::RendererDisplay(RendererOutput& output)
	: curDisplayType(RendererOutputType::Image), output(output) {}

char* RendererDisplay::getPixels() {
	switch (this->curDisplayType)
	{
	case RendererOutputType::Image:
		return reinterpret_cast<char*>(this->output.getOutput(RendererOutputType::Image).getBuffer());
	case RendererOutputType::Variance:
		return this->pixels.get();
	}
	return reinterpret_cast<char*>(this->output.getOutput(RendererOutputType::Image).getBuffer());
}

void RendererDisplay::setDisplayType(RendererOutputType type) {
	if (type == this->curDisplayType) return;
	this->curDisplayType = type;
	if (type == RendererOutputType::Image) return;
	if (type == RendererOutputType::Variance) {
		uint32_t size = 3 * getWorld().getSettings().width * getWorld().getSettings().height;
		this->pixels = std::make_unique<char[]>(size);
		float* data = reinterpret_cast<float*>(this->output.getOutput(RendererOutputType::Variance).getBuffer());
		for (uint32_t i = 0; i < size; ++i) {
			this->pixels[i] = static_cast<char>(255.f * sqrtf(data[i]));
		}
	}
}

