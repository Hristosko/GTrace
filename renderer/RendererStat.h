#pragma once

#include <cstdint>
#include <mutex>

class RendererStat {
public:
	void renderBegin();
	void updateStat(uint32_t samples, uint32_t rays);
	void renderFinish() const;
private:
	std::mutex mut;
	uint32_t mainSamples;
	uint32_t rayCasts;
};