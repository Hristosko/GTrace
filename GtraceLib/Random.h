#pragma once

#include <random>

namespace gtrace {

class RandomGenerator {
public:
    RandomGenerator() : rd(), gen(rd()), dist(0.f, 1.f) {}

    float get() {
        return dist(gen);
    }
private:
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> dist;
};
}
