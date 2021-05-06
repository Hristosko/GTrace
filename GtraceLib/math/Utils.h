#pragma once

#define PI   3.14159265359f
/// 1/PI
#define PI_1 0.31830988618f

inline float degToRad(float deg) {
	return PI * deg / 180.f;
}