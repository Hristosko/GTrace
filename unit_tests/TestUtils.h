#pragma once
#include "gtest/gtest.h"

#include "math/Vector3f.h"
#include "math/Matrix4x4.h"
#include "math/Transform.h"
#define EPS 0.001

using Test = ::testing::Test;

namespace gtrace
{
inline bool compare(float a, float b)
{
    return fabsf(a - b) < EPS;
}

bool compare(const Vector3f& a, const Vector3f& b, bool cmp4 = false);
bool compare(const Matrix4x4& a, const Matrix4x4& b);

inline bool operator==(const Vector3f& a, const Vector3f& b)
{
    return compare(a, b);
}

Vector3f randomVector3f();
}  // namespace gtrace