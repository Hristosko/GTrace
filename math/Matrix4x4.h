#pragma once

#include "Vector3f.h"

class Matrix4x4 {
private:
	union {
		__m128 rows[4];
		__m256 m256[2];
	};

private:
	explicit Matrix4x4(const __m256& x, const __m256& y) : m256{ x, y } {}

public:
	Matrix4x4(const float* elems)
		: rows{
		_mm_setr_ps(elems[0], elems[1], elems[2], elems[3]),
		_mm_setr_ps(elems[4], elems[5], elems[6], elems[7]),
		_mm_setr_ps(elems[8], elems[9], elems[10], elems[11]),
		_mm_setr_ps(elems[12], elems[13], elems[14], elems[15])
	} {}

	Vector3f operator[](int i) const {
		return this->rows[i];
	}

	friend __m256 twoRowsMult(const __m256& a01, const Matrix4x4& b);
	friend Matrix4x4 operator*(const Matrix4x4& a, const Matrix4x4& b);
};

inline __m256 twoRowsMult(const __m256& a01, const Matrix4x4& b) {
	__m256 result;
	result = _mm256_mul_ps(_mm256_shuffle_ps(a01, a01, _MM_SHUFFLE(0,0,0,0)), _mm256_broadcast_ps(&b.rows[0]));
	result = _mm256_add_ps(result, _mm256_mul_ps(_mm256_shuffle_ps(a01, a01, _MM_SHUFFLE(1, 1, 1, 1)), _mm256_broadcast_ps(&b.rows[1])));
	result = _mm256_add_ps(result, _mm256_mul_ps(_mm256_shuffle_ps(a01, a01, _MM_SHUFFLE(2, 2, 2, 2)), _mm256_broadcast_ps(&b.rows[2])));
	result = _mm256_add_ps(result, _mm256_mul_ps(_mm256_shuffle_ps(a01, a01, _MM_SHUFFLE(3, 3, 3, 3)), _mm256_broadcast_ps(&b.rows[3])));
	return result;
}


inline Matrix4x4 operator*(const Matrix4x4& a, const Matrix4x4& b) {
	__m256 out01 = twoRowsMult(a.m256[0], b);
	__m256 out23 = twoRowsMult(a.m256[1], b);
	return Matrix4x4(out01, out23);
}
