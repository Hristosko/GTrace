#pragma once

#include <cstdint>
#include <mutex>

namespace gtrace {

class World;

struct SecondaryRaysStat {
	uint32_t ligthSamples;

	SecondaryRaysStat();

	void addLightSample() { ++ligthSamples; }
};

class RendererStat {
public:
	void renderBegin();
	void updateStat(uint32_t samples, uint32_t rays);
	void updateStat(const SecondaryRaysStat& sr);
	void renderFinish(const World& w) const;
private:
	std::mutex mut;
	uint32_t mainSamples;
	uint32_t rayCasts;
	SecondaryRaysStat totalSecondaryRays;
};
}
