#include "RendererDisplay.h"

RendererDisplay::RendererDisplay(RendererOutput& output)
	: output(output) {}

char* RendererDisplay::getPixels() {
	return reinterpret_cast<char*>(this->output.getOutput(RendererOutputType::Image).getBuffer());
}

