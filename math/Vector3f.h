#pragma once
#include <xmmintrin.h>
#include <smmintrin.h>
#include <cmath>

///
/// Storing Vector3f in a clss that will be allocated on the heap
/// causes allignment issues.
/// Use this struct to store the data and make Vector3f for calculations
///
struct Vector3fData {
	float x, y, z;

	Vector3fData(float t = 0.f) : x(t), y(t), z(t) {}
	Vector3fData(float x, float y, float z) : x(x), y(y), z(z) {}
};

class Vector3f {
private:
	Vector3f(__m128 v) { this->vec = v; }
public:
	Vector3f() { this->vec = _mm_setzero_ps(); }
	explicit Vector3f(float v) { this->vec = _mm_setr_ps(v, v, v, 0.f); }
	Vector3f(float x, float y, float z) { this->vec = _mm_setr_ps(x, y, z, 0.f); }
	Vector3f(const Vector3fData vd) { this->vec = _mm_setr_ps(vd.x, vd.y, vd.z, 0.f); }

	float x() const {
		int res = _mm_extract_ps(this->vec, 0);
		return reinterpret_cast<float&>(res);
	}
	float y() const {
		int res = _mm_extract_ps(this->vec, 1);
		return reinterpret_cast<float&>(res);
	}
	float z() const {
		int res = _mm_extract_ps(this->vec, 2);
		return reinterpret_cast<float&>(res);
	}
	Vector3fData data() const {
		return Vector3fData(this->x(), this->y(), this->z());
	}

	friend Vector3f min(const Vector3f& a, const Vector3f& b);
	friend Vector3f max(const Vector3f& a, const Vector3f& b);
	friend Vector3f clamp(const Vector3f& a, const Vector3f& low, const Vector3f up);

	Vector3f operator-() const {
		return _mm_sub_ps(_mm_setzero_ps(), this->vec);
	}
	Vector3f& operator-=(const Vector3f& rhs) {
		this->vec = _mm_sub_ps(this->vec, rhs.vec);
		return *this;
	}
	Vector3f& operator+=(const Vector3f& rhs) {
		this->vec = _mm_add_ps(this->vec, rhs.vec);
		return *this;
	}
	Vector3f& operator*=(float a) {
		this->vec = _mm_mul_ps(this->vec, _mm_setr_ps(a, a, a, a));
		return *this;
	}
	Vector3f& operator*=(const Vector3f& a) {
		this->vec = _mm_mul_ps(this->vec, a.vec);
		return *this;
	}
	Vector3f& operator/=(float a) {
		this->vec = _mm_div_ps(this->vec, _mm_setr_ps(a, a, a, a));
		return *this;
	}
	Vector3f& operator /= (const Vector3f & a) {
		this->vec = _mm_div_ps(this->vec, a.vec);
		return *this;
	}

	friend float dot(const Vector3f& a, const Vector3f& b);
	friend Vector3f cross(const Vector3f& a, const Vector3f& b);

	void normalize() {
		const float c = 1.f / sqrtf(dot(*this, *this));
		*this *= c;
	}

	float lengthSqr() const {
		return dot(*this, *this);
	}

	float length() const {
		return sqrtf(this->lengthSqr());
	}

private:
	__m128 vec;
};

inline Vector3f min(const Vector3f& a, const Vector3f& b) {
	return _mm_min_ps(a.vec, b.vec);
}

inline Vector3f max(const Vector3f& a, const Vector3f& b) {
	return _mm_max_ps(a.vec, b.vec);
}

inline Vector3f clamp(const Vector3f& a, const Vector3f& low, const Vector3f up) {
	return max(low, min(a, up));
}

inline Vector3f clamp(const Vector3f& v, float low, float up) {
	return clamp(v, Vector3f(low), Vector3f(up));
}

inline Vector3f operator-(const Vector3f& a, const Vector3f& b) {
	Vector3f res = a;
	res -= b;
	return res;
}

inline Vector3f operator+(const Vector3f& a, const Vector3f& b) {
	Vector3f res = a;
	res += b;
	return res;
}

inline Vector3f operator*(const Vector3f& a, float b) {
	Vector3f res = a;
	res *= b;
	return res;
}

inline Vector3f operator*(float b, const Vector3f& a) {
	Vector3f res = a;
	res *= b;
	return res;
}

inline Vector3f operator*(const Vector3f& a, const Vector3f& b) {
	Vector3f res = a;
	res *= b;
	return res;
}

inline Vector3f operator/(const Vector3f& a, float b) {
	Vector3f res = a;
	res /= b;
	return res;
}

inline Vector3f operator/(const Vector3f& a, const Vector3f& b) {
	Vector3f res = a;
	res /= b;
	return res;
}

inline float dot(const Vector3f& a, const Vector3f& b) {
	Vector3f r = a * b;
	union {
		float comp[3];
		__m128 m128;
	} dummy;
	dummy.m128 = r.vec;
	return dummy.comp[0] + dummy.comp[1] + dummy.comp[2];
}

inline Vector3f cross(const Vector3f& a, const Vector3f& b) {
	__m128 res = _mm_sub_ps(
		_mm_mul_ps(a.vec, _mm_shuffle_ps(b.vec, b.vec, _MM_SHUFFLE(3, 0, 2, 1))),
		_mm_mul_ps(b.vec, _mm_shuffle_ps(a.vec, a.vec, _MM_SHUFFLE(3, 0, 2, 1)))
	);
	return _mm_shuffle_ps(res, res, _MM_SHUFFLE(3, 0, 2, 1));
}

inline Vector3f normalize(const Vector3f& v) {
	Vector3f res = v;
	res.normalize();
	return res;
}