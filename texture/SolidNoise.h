#pragma once

#include <cstdint>

class SolidNoise {
public:
	SolidNoise();
private:
	float omega(float t) const;
	//float 
private:
	int8_t phi[16];
};