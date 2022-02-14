#pragma once

#include "common/HeapAligned.h"
#include "Matrix4x4.h"
#include "Utils.h"

namespace gtrace
{
class Transform : public HeapAlignedType<Matrix4x4>
{
public:
    Transform(const Matrix4x4& mat = Transform::makeIdentity()) :
        T(mat.transposed()),  // we store the transposed matrix so that we can eff. transform vectors
        iT(mat.inversed().transposed())
    {
    }

    const Matrix4x4& get() const { return this->T; }
    const Matrix4x4& getInv() const { return this->iT; }

    Vector3f transform(const Vector3f& v) const { return this->transform(this->T, v); }
    Vector3f invTransform(const Vector3f& v) const { return this->transform(this->iT, v); }

    Vector3f transformDirection(const Vector3f& v) const { return this->transformDirection(this->T, v); }
    Vector3f invTransformDirection(const Vector3f& v) const { return this->transformDirection(this->iT, v); }

    static Matrix4x4 makeTranslation(const Vector3f& v)
    {
        return Matrix4x4(
            _mm_setr_ps(1.f, 0.f, 0.f, v.x()), _mm_setr_ps(0.f, 1.f, 0.f, v.y()), _mm_setr_ps(0.f, 0.f, 1.f, v.z()),
            _mm_setr_ps(0.f, 0.f, 0.f, 1.f));
    }

    static Matrix4x4 makeScale(float k)
    {
        return Matrix4x4(
            _mm_setr_ps(k, 0.f, 0.f, 0.f), _mm_setr_ps(0.f, k, 0.f, 0.f), _mm_setr_ps(0.f, 0.f, k, 0.f),
            _mm_setr_ps(0.f, 0.f, 0.f, 1.f));
    }

    static Matrix4x4 makeRotationX(float x)
    {
        x = degToRad(x);
        const float sx = sinf(x);
        const float cx = cosf(x);
        return Matrix4x4(
            _mm_setr_ps(1.f, 0.f, 0.f, 0.f), _mm_setr_ps(0.f, cx, -sx, 0.f), _mm_setr_ps(0.f, sx, cx, 0.f),
            _mm_setr_ps(0.f, 0.f, 0.f, 1.f));
    }

    static Matrix4x4 makeRotationY(float x)
    {
        x = degToRad(x);
        const float sx = sinf(x);
        const float cx = cosf(x);
        return Matrix4x4(
            _mm_setr_ps(cx, 0.f, sx, 0.f), _mm_setr_ps(0.f, 1.f, 0.f, 0.f), _mm_setr_ps(-sx, 0.f, cx, 0.f),
            _mm_setr_ps(0.f, 0.f, 0.f, 1.f));
    }

    static Matrix4x4 makeRotationZ(float x)
    {
        x = degToRad(x);
        const float sx = sinf(x);
        const float cx = cosf(x);
        return Matrix4x4(
            _mm_setr_ps(cx, -sx, 0.f, 0.f), _mm_setr_ps(sx, cx, 0.f, 0.f), _mm_setr_ps(0.f, 0.f, 1.f, 0.f),
            _mm_setr_ps(0.f, 0.f, 0.f, 1.f));
    }

    static Matrix4x4 makeBasisChange(const Vector3f& u, const Vector3f& v, const Vector3f& w)
    {
        return Matrix4x4(
            _mm_setr_ps(u.x(), u.y(), u.z(), 0.f), _mm_setr_ps(v.x(), v.y(), v.z(), 0.f),
            _mm_setr_ps(w.x(), w.y(), w.z(), 0.f), _mm_setr_ps(0.f, 0.f, 0.f, 1.f));
    }

    static Matrix4x4 makeIdentity() { return Transform::makeScale(1.f); }

    static Matrix4x4 makeRotation(float x, float y, float z)
    {
        return makeRotationX(x) * makeRotationY(y) * makeRotationZ(z);
    }

private:
    Vector3f transform(const Matrix4x4& m, const Vector3f& v) const
    {
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

    Vector3f transformDirection(const Matrix4x4& m, const Vector3f& v) const
    {
        // make the 4rth el. 1
        MM128 un;
        un.m128 = v.vec;
        un.f[3] = 0.f;

        __m128 res = _mm_mul_ps(m.rows[0], _mm_broadcast_ss(&un.f[0]));
        res = _mm_add_ps(res, _mm_mul_ps(m.rows[1], _mm_broadcast_ss(&un.f[1])));
        res = _mm_add_ps(res, _mm_mul_ps(m.rows[2], _mm_broadcast_ss(&un.f[2])));
        res = _mm_add_ps(res, _mm_mul_ps(m.rows[3], _mm_broadcast_ss(&un.f[3])));
        return normalize(Vector3f(res));
    }

private:
    Matrix4x4 T;   // The matrix that represents the transform
    Matrix4x4 iT;  // The inverse matrix i.e. the inverse transform
};
}  // namespace gtrace
