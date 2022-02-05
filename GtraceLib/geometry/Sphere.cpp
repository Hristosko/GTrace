#include "Sphere.h"
#include "BBox.h"

namespace gtrace {

Sphere::Sphere(float radius, Material* material) :
    Shape(std::make_shared<Transform>()),
    radius(radius),
    mat(material) {}

void Sphere::parse (const SceneParser& parser, std::unordered_map<std::string, std::string>& map) {
    this->parseTransform(parser, map);
    parser.parsefloatAndStore(map, "radius", this->radius);
    parser.parseMaterialAndStore(map, "mat", this->mat);
}

bool Sphere::hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const {
    if (rec.shape == this) return false;
    const Vector3f origin = objectToWorld->invTransform(ray.origin);
    const Vector3f direction = objectToWorld->invTransformDirection(ray.direction);
    const float a = dot(direction, direction);
    const float b = 2.f * dot(direction, origin);
    const float c = dot(origin, origin) - radius * radius;

    float discr = b * b - 4.f * a * c;
    if (discr > 0.f) {
        discr = sqrtf(discr);
        float t = (-b - discr) / (2.f * a);
        if (t < tmin) {
            t = (-b + discr) / (2.f * a);
        }
        if (t < tmin || t > tmax) return false;

        // we have a hit
        rec.normal = origin + t * direction;
        rec.normal = this->objectToWorld->transformDirection(rec.normal);
        rec.update(t, this->mat, this);
        return true;
    }
    return false;
}

BBox Sphere::bbox() const {
    const Vector3f rad(this->radius);
    const Vector3f center = this->objectToWorld->transform(Vector3f(0.f));
    return BBox(
        center - rad,
        center + rad);
}
}
