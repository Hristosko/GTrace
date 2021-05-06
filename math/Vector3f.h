#pragma once
#include <xmmintrin.h>
#include <smmintrin.h>
#include <immintrin.h>
#include <cmath>

union MM128 {
	__m128 m128;
	float f[4];
};

class Vector3f {
public:
	friend class Matrix4x4;
	friend class Transform;
	Vector3f(__m128 v) { this->vec = v; }
	Vector3f() { this->vec = _mm_setzero_ps(); }
	explicit Vector3f(float v) { this->vec = _mm_setr_ps(v, v, v, 0.f); }
	Vector3f(float x, float y, float z) { this->vec = _mm_setr_ps(x, y, z, 0.f); }

	float x() const {
		return (*this)[0];
	}
	float y() const {
		return (*this)[1];
	}
	float z() const {
		return (*this)[2];
	}
	float operator[](int i) const {
		__m128i vidx = _mm_cvtsi32_si128(i);
		__m128 shuffled = _mm_permutevar_ps(this->vec, vidx);
		return _mm_cvtss_f32(shuffled);
	}

	friend Vector3f min(const Vector3f& a, const Vector3f& b);
	friend Vector3f max(const Vector3f& a, const Vector3f& b);
	friend Vector3f clamp(const Vector3f& a, const Vector3f& low, const Vector3f up);

	friend float maxElement(const Vector3f& a, float b);
	friend float minElement(const Vector3f& a, float b);

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

	bool operator<=(const Vector3f& a) const {
		const __m128 comp = _mm_cmple_ps(this->vec, a.vec);
		return _mm_movemask_ps(comp) == 0xF;
	}
	bool operator<(const Vector3f& a) const {
		const __m128 comp = _mm_cmplt_ps(this->vec, a.vec);
		return _mm_movemask_ps(comp) >= 0x7; // we aren't intersted in the 4th element
	}

	bool isAtLeastOneGreaterThan(float a) const {
		const __m128 av = _mm_setr_ps(a, a, a, a);
		const __m128 comp = _mm_cmpgt_ps(this->vec, av);
		const int res = _mm_movemask_ps(comp);
		return  res & 0x7;
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

	void sqrt() {
		this->vec = _mm_sqrt_ps(this->vec);
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

inline Vector3f sqrt(const Vector3f& v) {
	Vector3f res = v;
	res.sqrt();
	return res;
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

inline float maxElement(const Vector3f& a, float b) {
	MM128 mm;
	mm.m128 = a.vec;
	mm.f[3] = b;
	// contains the elements of a and b as 4-th element
	const __m128 vec = mm.m128;
	const __m128 rotBy1 = _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(2, 1, 0, 3));
	const __m128 vec2 = _mm_max_ps(vec, rotBy1);
	const __m128 rotBy2 = _mm_shuffle_ps(vec2, vec2, _MM_SHUFFLE(1, 0, 3, 2));
	const __m128 vecMax = _mm_max_ps(vec2, rotBy2);
	return _mm_cvtss_f32(vecMax);
}

inline float minElement(const Vector3f& a, float b) {
	MM128 mm;
	mm.m128 = a.vec;
	mm.f[3] = b;
	// contains the elements of a and b as 4-th element
	const __m128 vec = mm.m128;
	const __m128 rotBy1 = _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(2, 1, 0, 3));
	const __m128 vec2 = _mm_min_ps(vec, rotBy1);
	const __m128 rotBy2 = _mm_shuffle_ps(vec2, vec2, _MM_SHUFFLE(1, 0, 3, 2));
	const __m128 vecMin = _mm_min_ps(vec2, rotBy2);
	return _mm_cvtss_f32(vecMin);
}