#pragma once

#include <cstdint>
#include <mutex>

namespace gtrace {

class World;

struct RayTracingStat {
    uint32_t ligthSamples;

    RayTracingStat();

    void addLightSample() { ++ligthSamples; }
};

class RendererStat {
public:
    void renderBegin();
    void updateStat(uint32_t samples, uint32_t rays);
    void updateStat(const RayTracingStat& sr);
    void renderFinish(const World& w) const;
private:
    std::mutex mut;
    uint32_t mainSamples;
    uint32_t rayCasts;
    RayTracingStat totalSecondaryRays;
};
}
