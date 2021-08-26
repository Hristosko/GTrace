#include "RendererDisplay.h"
#include "scene/World.h"
#include "Errors.h"

namespace gtrace {

RendererDisplay::RendererDisplay(RendererOutput& output)
	: curDisplayType(RendererOutputType::Image), output(output) {}

char* RendererDisplay::getPixels() {
	return this->pixels.get();
}

void RendererDisplay::setDisplayType(const World& w, RendererOutputType type) {
	if (type == this->curDisplayType) return;
	this->curDisplayType = type;
	this->updateDisplay(w);
}

void RendererDisplay::updateDisplay(const World& w) {
	if (this->curDisplayType == RendererOutputType::Image) {
		uint32_t size = 3 * w.getSettings().width * w.getSettings().height;
		this->pixels = std::make_unique<char[]>(size);
		float* data = reinterpret_cast<float*>(this->output.getOutput(RendererOutputType::Image).getBuffer());
		for (uint32_t i = 0; i < size; ++i) {
			this->pixels[i] = static_cast<char>(255.f * data[i]);
		}
	}
	if (this->curDisplayType == RendererOutputType::Variance) {
		uint32_t size = 3 * w.getSettings().width * w.getSettings().height;
		this->pixels = std::make_unique<char[]>(size);
		float* data = reinterpret_cast<float*>(this->output.getOutput(RendererOutputType::Variance).getBuffer());
		for (uint32_t i = 0; i < size; ++i) {
			this->pixels[i] = static_cast<char>(255.f * sqrtf(data[i]));
		}
	}
}

}
