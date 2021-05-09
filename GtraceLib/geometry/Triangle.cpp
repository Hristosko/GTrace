#include "Triangle.h"
#include "BBox.h"

namespace gtrace {

/**
 * Utility fiunction used by the triangle and by the meshes for intersectig a triangle with ray.
 * @param a, b, c The verteces of the triangle
 * @param tr The transformation of the world coordinates to obj coordinates
 * @param ray The ray
 * @param tmin, tmax See Shape::hit
 * @param beta, gamma Used to calculate the normal for a mesh with normals provided by the user
 * @param tval The point of intersection (if any)
 * @return true if there is an intersection, false otherwise
 */
bool Triangle::hit(const Vector3f& a, const Vector3f& b, const Vector3f& c, const Transform* tr,
	const Ray& ray, float tmin, float tmax,
	float& beta, float& gamma, float& tval) {
	const Vector3f origin = tr->invTransform(ray.origin);
	const Vector3f direction = tr->invTransformDirection(ray.direction);

	Vector3f AB = Vector3f(a) - Vector3f(b);
	Vector3f AC = Vector3f(a) - Vector3f(c);
	Vector3f AO = Vector3f(a) - origin;

	const float E = AC.y();
	const float I = direction.z();
	const float H = direction.y();
	const float F = AC.z();
	const float EIHF = E * I - H * F;

	const float G = direction.x();
	const float D = AC.x();
	const float GFDI = G * F - D * I;

	const float DHEG = D * H - E * G;

	Vector3f temp(EIHF, GFDI, DHEG);
	const float denom = dot(AB, temp);
	beta = dot(AO, temp) / denom;

	if (beta <= 0.f || beta >= 1.f) return false;

	const float A = AB.x();
	const float B = AB.y();
	const float C = AB.z();

	const float J = AO.x();
	const float K = AO.y();
	const float L = AO.z();

	const float AKJB = A * K - J * B;
	const float JCAL = J * C - A * L;
	const float BLKC = B * L - K * C;

	temp = Vector3f(BLKC, JCAL, AKJB);
	gamma = dot(temp, direction) / denom;
	if (gamma <= 0.f || beta + gamma >= 1.f) return false;

	tval = -dot(temp, AC) / denom;
	return tval >= tmin && tval <= tmax;
}

/**
 * Utility fiunction that returns a bbox for a triangle.
 */
BBox Triangle::triangleBBox(const Vector3f& a, const Vector3f& b, const Vector3f& c, const Transform* tr) {
	const Vector3f A = tr->transform(a);
	const Vector3f B = tr->transform(b);
	const Vector3f C = tr->transform(c);
	const Vector3f bottom = min(min(A, B), C);
	const Vector3f top = max(max(A, B), C);
	return BBox(bottom, top);
}

void Triangle::parse(const SceneParser& parser, std::unordered_map<std::string, std::string>& map) {
	parser.parseVector3fAndStore(map, "a", this->a);
	parser.parseVector3fAndStore(map, "b", this->b);
	parser.parseVector3fAndStore(map, "c", this->c);
	parser.parseMaterialAndStore(map, "mat", this->mat);
	this->parseTransform(parser, map);

}

bool Triangle::hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const {
	if (rec.shape == this) return false;
	float tval, beta, gamma;

	if (Triangle::hit(a, b, c, this->objectToWorld.get(), ray, tmin, tmax, beta, gamma, tval)) {
		rec.update(tval, this->mat, this);
		rec.normal = this->objectToWorld->invTransformDirection(cross(b - a, c - a));
		if (dot(rec.normal, ray.direction) > 0.f) rec.normal *= Vector3f(-1.f);
		return true;
	}
	return false;
}

BBox Triangle::bbox() const {
	return Triangle::triangleBBox(this->a, this->b, this->c, this->objectToWorld.get());
}
}
