#pragma once

#include <cstdint>
#include <mutex>

class World;
class RendererStat {
public:
	void renderBegin();
	void updateStat(uint32_t samples, uint32_t rays);
	void renderFinish(const World& w) const;
private:
	std::mutex mut;
	uint32_t mainSamples;
	uint32_t rayCasts;
};