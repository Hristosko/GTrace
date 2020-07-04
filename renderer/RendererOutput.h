#pragma once

#include "DataBuffer.h"

struct ColorResult {
	char r, g, b;
};

enum RendererOutputType : uint16_t {
	Image = 0,
	Count
};

class RendererOutput {
public:
	DataBuffer& getOutput(RendererOutputType type) {
		return this->outputs[type];
	}
	void init();

private:
	void initImageOutput();
private:
	DataBuffer outputs[RendererOutputType::Count];
};