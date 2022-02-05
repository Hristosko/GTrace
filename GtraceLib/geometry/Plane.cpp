#include "Plane.h"
#include "BBox.h"

namespace gtrace {

void Plane::parse (const SceneParser& parser, std::unordered_map<std::string, std::string>& map) {
    this->parseTransform(parser, map);
    parser.parsefloatAndStore(map, "x", this->x);
    parser.parsefloatAndStore(map, "y", this->y);
    this->x *= 0.5f;
    this->y *= 0.5f;
    parser.parseMaterialAndStore(map, "mat", this->mat);
}

bool Plane::hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const {
    if (rec.shape == this) return false;
    const Vector3f origin = objectToWorld->invTransform(ray.origin);
    const Vector3f direction = objectToWorld->invTransformDirection(ray.direction);

    if (direction.z() == 0) return false;
    const float t = -origin.z() / direction.z();
    if (t < tmin || t > tmax) return false;

    Vector3f hitPosition = origin + t*direction;
    if (fabs(hitPosition.x()) > this->x || fabs(hitPosition.y()) > this->y) return false;

    rec.normal = this->objectToWorld->transformDirection(Vector3f(0.f, 0.f, 1.f));
    rec.update(t, this->mat, this);
    return true;
}

BBox Plane::bbox() const {
    const Vector3f top = this->objectToWorld->transform(Vector3f(this->x, this->y, 0.f));
    return BBox(-top, top);
}
}
