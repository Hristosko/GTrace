#pragma once

#include "Vector3f.h"

class Matrix4x4 {
private:
	union {
		__m128 rows[4];
		__m256 m256[2];
	};

private:
	explicit Matrix4x4(const __m128& a, const __m128& b, const __m128& c, const __m128& d)
		: rows{ a, b, c, d } {}
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
	Matrix4x4 transposed() const {
		/*
		0  1  2  3
		4  5  6  7
		8  9  10 11
		12 13 14 15
		*/
		/*
		->
		0  1  4  5
		2  3  6  7
		8  9  12 13
		10 11 14 15
		*/
		const __m128 a0 = _mm_shuffle_ps(rows[0], rows[1], _MM_SHUFFLE(1, 0, 1, 0));
		const __m128 b0 = _mm_shuffle_ps(rows[0], rows[1], _MM_SHUFFLE(3, 2, 3, 2));
		const __m128 c0 = _mm_shuffle_ps(rows[2], rows[3], _MM_SHUFFLE(1, 0, 1, 0));
		const __m128 d0 = _mm_shuffle_ps(rows[2], rows[3], _MM_SHUFFLE(3, 2, 3, 2));

		/*
		->
		0  4  8  12
		1  5  9  13
		2  6  10 14
		3  7  11 15
		*/
		const __m128 a1 = _mm_shuffle_ps(a0, c0, _MM_SHUFFLE(2, 0, 2, 0));
		const __m128 b1 = _mm_shuffle_ps(a0, c0, _MM_SHUFFLE(3, 1, 3, 1));
		const __m128 c1 = _mm_shuffle_ps(b0, d0, _MM_SHUFFLE(2, 0, 2, 0));
		const __m128 d1 = _mm_shuffle_ps(b0, d0, _MM_SHUFFLE(3, 1, 3, 1));

		return Matrix4x4(a1, b1, c1, d1);
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
