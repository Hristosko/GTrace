#pragma once

#include "RendererOutput.h"
#include "../UniqueBuffer.h"

class RendererDisplay {
public:
	RendererDisplay(RendererOutput& output);

	char* getPixels();

	void setDisplayType(RendererOutputType type);

private:
	RendererOutputType curDisplayType;
	RendererOutput& output;
	UniqueBufferPtr<char> pixels;
};