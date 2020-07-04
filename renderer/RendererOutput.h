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
	void init(uint32_t width, uint32_t height);

	uint32_t getWidth() { return this->width; }
	uint32_t getHeight() { return this->height; }
private:
	void initImageOutput();
private:
	uint32_t width, height;
	DataBuffer outputs[RendererOutputType::Count];
};