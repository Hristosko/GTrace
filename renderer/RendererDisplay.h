#pragma once

#include "RendererOutput.h"
#include <memory>

class RendererDisplay {
public:
	RendererDisplay(RendererOutput& output);

	char* getPixels();

	void setDisplayType(RendererOutputType type);

private:
	RendererOutputType curDisplayType;
	RendererOutput& output;
	std::unique_ptr<char[]> pixels;
};