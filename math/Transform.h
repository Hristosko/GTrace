#pragma once

#include "../HeapAligned.h"
#include "../Ref.h"
#include "Matrix4x4.h"

class Transform : public HeapAligned<32>, public ReferenceCounted {
public:
	Transform(const Matrix4x4& mat)
		: T(mat.transposed()), // we store the transposed matrix so that we can eff. transform vectors
		  iT(mat.inversed().transposed()) {}

	const Matrix4x4& get() const { return this->T; }
	const Matrix4x4& getInv() const { return this->iT; }

	Vector3f transform(const Vector3f& v) const {
		return this->transform(this->T, v);
	}
	Vector3f invTransform(const Vector3f& v) const {
		return this->transform(this->iT, v);
	}

private:
	Vector3f transform(const Matrix4x4& m, const Vector3f& v) const {
		// make the 4rth el. 1
		MM128 un;
		un.m128 = v.vec;
		un.f[3] = 1.f;

		__m128 res = _mm_mul_ps(m.rows[0], _mm_broadcast_ss(&un.f[0]));
		res = _mm_add_ps(res, _mm_mul_ps(m.rows[1], _mm_broadcast_ss(&un.f[1])));
		res = _mm_add_ps(res, _mm_mul_ps(m.rows[2], _mm_broadcast_ss(&un.f[2])));
		res = _mm_add_ps(res, _mm_mul_ps(m.rows[3], _mm_broadcast_ss(&un.f[3])));
		// make the 4th comp 1
		res = _mm_div_ps(res, _mm_shuffle_ps(res, res, _MM_SHUFFLE(3, 3, 3, 3)));
		return res;
	}

private:
	Matrix4x4 T; // The matrix that represents the transform
	Matrix4x4 iT; // The inverse matrix i.e. the inverse transform
};