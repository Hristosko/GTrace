#pragma once

#include "Vector3f.h"

namespace gtrace {

class Vector2f {
public:
	Vector2f() = default;
	Vector2f(float x) : vec(x, x, 0.f) {}
	Vector2f(float x, float y) : vec(x, y, 0) {}
	explicit Vector2f(const Vector3f& v) : vec(v) {}

	float x() const { return this->vec.x(); }
	float y() const { return this->vec.y(); }

	Vector2f& operator+=(const Vector2f& a) {
		this->vec += a.vec;
		return *this;
	}

	Vector2f& operator-=(const Vector2f& a) {
		this->vec -= a.vec;
		return *this;
	}

	Vector2f& operator*=(float a) {
		this->vec *= Vector3f(a);
		return *this;
	}
private:
	Vector3f vec;
};

using Point2f = Vector2f;

inline Vector2f operator+(const Vector2f& a, const Vector2f& b) {
	Vector2f res = a;
	res += b;
	return res;
}

inline Vector2f operator-(const Vector2f& a, const Vector2f& b) {
	Vector2f res = a;
	res -= b;
	return res;
}

inline Vector2f operator*(const Vector2f& a, float f) {
	Vector2f res = a;
	res *= f;
	return res;
}

inline Vector2f operator*(float f, const Vector2f& a) {
	return a * f;
}
}
