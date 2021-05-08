#pragma once

#include "Vector3f.h"

namespace gtrace {

class Matrix4x4 {
private:
	friend class Transform;
	union {
		__m128 rows[4];
		__m256 m256[2];
	};

private:
	explicit Matrix4x4(const __m128& a, const __m128& b, const __m128& c, const __m128& d)
		: rows{ a, b, c, d } {}
	explicit Matrix4x4(const __m256& x, const __m256& y) : m256{ x, y } {}

public:
	Matrix4x4()
		: rows {
		_mm_setr_ps(1.f, 0.f, 0.f, 0.f),
		_mm_setr_ps(0.f, 1.f, 0.f, 0.f),
		_mm_setr_ps(0.f, 0.f, 1.f, 0.f),
		_mm_setr_ps(0.f, 0.f, 0.f, 1.f)
	} {}
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
	Matrix4x4 inversed() const;
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
/// a*b
inline __m128 mult2x2(const __m128& a, const __m128& b) {
	return _mm_add_ps(
		_mm_mul_ps(a, _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 0, 3, 0))),
		_mm_mul_ps(_mm_shuffle_ps(a, a, _MM_SHUFFLE(2,3,0,1)),
				   _mm_shuffle_ps(b, b, _MM_SHUFFLE(1,2,1,2))));
}

/// a# * b
inline __m128 multAdj2x2(const __m128& a, const __m128& b) {
	return _mm_sub_ps(
		_mm_mul_ps(_mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 3, 3)), b),
		_mm_mul_ps(_mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 2, 1, 1)),
			_mm_shuffle_ps(b, b, _MM_SHUFFLE(1, 0, 3, 2))));
}

/// a * b#
inline __m128 mult2xAdj2(const __m128& a, const __m128& b) {
	return _mm_sub_ps(
		_mm_mul_ps(a, _mm_shuffle_ps(b, b, _MM_SHUFFLE(0, 3, 0, 3))),
		_mm_mul_ps(_mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 3, 0, 1)),
			_mm_shuffle_ps(b, b, _MM_SHUFFLE(1, 2, 1, 2))));
}

inline Matrix4x4 Matrix4x4::inversed() const {
	// The 4 sub-matrices
	const __m128 A = _mm_shuffle_ps(rows[0], rows[1], _MM_SHUFFLE(1, 0, 1, 0));
	const __m128 B = _mm_shuffle_ps(rows[0], rows[1], _MM_SHUFFLE(3, 2, 3, 2));
	const __m128 C = _mm_shuffle_ps(rows[2], rows[3], _MM_SHUFFLE(1, 0, 1, 0));
	const __m128 D = _mm_shuffle_ps(rows[2], rows[3], _MM_SHUFFLE(3, 2, 3, 2));

	// vector |A| |B| |C| |D|
	const __m128 det = _mm_sub_ps(
		_mm_mul_ps(_mm_shuffle_ps(rows[0], rows[2], _MM_SHUFFLE(2,0,2,0)),
				   _mm_shuffle_ps(rows[1], rows[3], _MM_SHUFFLE(3,1,3,1))),
		_mm_mul_ps(_mm_shuffle_ps(rows[0], rows[2], _MM_SHUFFLE(3,1,3,1)),
			       _mm_shuffle_ps(rows[1], rows[3], _MM_SHUFFLE(2,0,2,0))));

	const __m128 detA = _mm_shuffle_ps(det, det, _MM_SHUFFLE(0, 0, 0, 0));
	const __m128 detB = _mm_shuffle_ps(det, det, _MM_SHUFFLE(1, 1, 1, 1));
	const __m128 detC = _mm_shuffle_ps(det, det, _MM_SHUFFLE(2, 2, 2, 2));
	const __m128 detD = _mm_shuffle_ps(det, det, _MM_SHUFFLE(3, 3, 3, 3));

	// D#C and A#B
	const __m128 DC = multAdj2x2(D, C);
	const __m128 AB = multAdj2x2(A, B);

	// iM = 1/|M| * | X  Y |
	//              | Z  W |

	// X# = |D|A - B(D#C)
	const __m128 X_ = _mm_sub_ps(_mm_mul_ps(detD, A), mult2x2(B, DC));
	// W# = |A|D - C(A#B)
	const __m128 W_ = _mm_sub_ps(_mm_mul_ps(detA, D), mult2x2(C, AB));

	// Y# = |B|C - D(A#B)#
	const __m128 Y_ = _mm_sub_ps(_mm_mul_ps(detB, C), mult2xAdj2(D, AB));
	// Z# = |C|B - A(D#C)#
	const __m128 Z_ = _mm_sub_ps(_mm_mul_ps(detC, B), mult2xAdj2(A, DC));

	// trace((A#B)(D#C))
	__m128 tr = _mm_mul_ps(AB, _mm_shuffle_ps(DC, DC, _MM_SHUFFLE(3,1,2,0)));
	tr = _mm_hadd_ps(tr, tr);
	tr = _mm_hadd_ps(tr, tr);

	__m128 detM = _mm_add_ps(_mm_mul_ps(detA, detD), _mm_mul_ps(detB, detC));
	detM = _mm_sub_ps(detM, tr);

	const __m128 adjSignMask = _mm_setr_ps(1.f, -1.f, -1.f, 1.f);
	// (1/|M|, -1/|M|, -1/|M|, 1/|M|)
	__m128 rDetM = _mm_div_ps(adjSignMask, detM);

	const __m128 X = _mm_mul_ps(X_, rDetM);
	const __m128 Y = _mm_mul_ps(Y_, rDetM);
	const __m128 Z = _mm_mul_ps(Z_, rDetM);
	const __m128 W = _mm_mul_ps(W_, rDetM);

	// The sub-matrices of the result
	const __m128 AR = _mm_shuffle_ps(X, Y, _MM_SHUFFLE(1, 3, 1, 3));
	const __m128 BR = _mm_shuffle_ps(X, Y, _MM_SHUFFLE(0, 2, 0, 2));
	const __m128 CR = _mm_shuffle_ps(Z, W, _MM_SHUFFLE(1, 3, 1, 3));
	const __m128 DR = _mm_shuffle_ps(Z, W, _MM_SHUFFLE(0, 2, 0, 2));
	return Matrix4x4(AR, BR, CR, DR);
}
}
