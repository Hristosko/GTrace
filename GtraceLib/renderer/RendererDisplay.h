#pragma once

#include "RendererOutput.h"
#include "../scene/World.h"
#include <memory>

namespace gtrace {

class RendererDisplay {
public:
	RendererDisplay(RendererOutput& output);

	char* getPixels();

	void setDisplayType(const World& w, RendererOutputType type);

private:
	RendererOutputType curDisplayType;
	RendererOutput& output;
	std::unique_ptr<char[]> pixels;
};

}
