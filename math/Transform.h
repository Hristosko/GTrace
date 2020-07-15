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
private:
	Matrix4x4 T; // The matrix that represents the transform
	Matrix4x4 iT; // The inverse matrix i.e. the inverse transform
};