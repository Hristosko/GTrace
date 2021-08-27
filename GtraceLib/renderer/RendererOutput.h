#pragma once

#include "DataBuffer.h"

namespace gtrace {

struct ColorResult {
	float r, g, b;
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
		return this->buff.outputs[type];
	}
	void init();

	void save(const char* path);
	void open(const char* path);

	struct OutputBuffer {
		DataBuffer outputs[RendererOutputType::Count];

		void init(uint32_t w, uint32_t h);
	};

	static OutputBuffer getOutputBuffer(uint32_t w, uint32_t h);
	void update(const OutputBuffer& b, uint32_t offsetw, uint32_t offseth, uint32_t sizew, uint32_t sizeh);

private:
	World& world;
	OutputBuffer buff;
};
}
