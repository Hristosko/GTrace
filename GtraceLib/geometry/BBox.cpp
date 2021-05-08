#include "BBox.h"

namespace gtrace {

/**
 * Returns a BBox that bounds two BBoxxes
 * @param a The first bbox
 * @param b The second bbox
 */
BBox BBox::bound(const BBox& a, const BBox& b) {
	return BBox(
		gtrace::min(a.bottom, b.bottom),
		gtrace::max(a.top, b.top)
	);
}

/**
 * Intersts a ray with the BBox.
 * @parm ray The ray.
 * @param tmin The first point on the ray that can be part of the intersection
 * @param tmax The last point on the ray that can be part of the intersection
 * The intersection point t muss be in the interval [tmin, tmax]
 */
bool BBox::intersect(const Ray& ray, float tmin, float tmax) const {
	const Vector3f t0 = (this->bottom - ray.origin) * ray.invDirection;
	const Vector3f t1 = (this->top - ray.origin) * ray.invDirection;
	const Vector3f tsmaller = gtrace::min(t0, t1);
	const Vector3f tbigger = gtrace::max(t0, t1);

	tmin = maxElement(tsmaller, tmin);
	tmax = minElement(tbigger, tmax);
	return tmin <= tmax;
}
}
