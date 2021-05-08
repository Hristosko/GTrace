#pragma once

#include "Vector3f.h"

namespace gtrace {

struct OB_fromU {};
struct OB_fromV {};
struct OB_fromW {};
struct OB_fromUV {};
struct OB_fromVW {};
struct OB_fromUW {};
struct OB_fromWV {};


class OrthonormalBasis {
public:
	OrthonormalBasis() = default;
	OrthonormalBasis(const Vector3f& a, OB_fromU);
	OrthonormalBasis(const Vector3f& a, OB_fromV);
	OrthonormalBasis(const Vector3f& a, OB_fromW);

	OrthonormalBasis(const Vector3f& a, const Vector3f& b, OB_fromUV);
	OrthonormalBasis(const Vector3f& a, const Vector3f& b, OB_fromUW);
	OrthonormalBasis(const Vector3f& a, const Vector3f& b, OB_fromVW);
	OrthonormalBasis(const Vector3f& a, const Vector3f& b, OB_fromWV);



	const Vector3f& u() const { return vec[0]; }
	const Vector3f& v() const { return vec[1]; }
	const Vector3f& w() const { return vec[2]; }

private:
	Vector3f vec[3];
};
}
