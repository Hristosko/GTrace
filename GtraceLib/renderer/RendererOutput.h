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

class World;
class RendererOutput {
public:
	RendererOutput(World& w) : world(w) {}
	DataBuffer& getOutput(RendererOutputType type) {
		return this->outputs[type];
	}
	void init();

	void save(const char* path);
	void open(const char* path);

private:
	void initImageOutput();
	void initVarianceOutput();
private:
	World& world;
	DataBuffer outputs[RendererOutputType::Count];
};
