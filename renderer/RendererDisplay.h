#pragma once

#include "RendererOutput.h"
#include <memory>

class RendererDisplay {
public:
	RendererDisplay(RendererOutput& output);

	char* getPixels();
private:
	RendererOutput& output;
	std::unique_ptr<char[]> pixels;
};