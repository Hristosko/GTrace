#pragma once

#include "DataBuffer.h"

struct ColorResult {
	char r, g, b;
};

struct VarianceResult {
	float x, y, z;
};

enum RendererOutputType : uint16_t {
	Image = 0,
	Variance,
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
	void initVarianceOutput();
private:
	DataBuffer outputs[RendererOutputType::Count];
};