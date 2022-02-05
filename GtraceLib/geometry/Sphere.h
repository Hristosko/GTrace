#pragma once

#include "Shape.h"

namespace gtrace {

class Sphere : public Shape {
public:
    Sphere() = default;
    explicit Sphere(float radius, Material* material = nullptr);

    virtual void parse(const SceneParser& parser, std::unordered_map<std::string, std::string>& map) override;
    virtual bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const override;
    virtual BBox bbox() const override;

private:
    float radius; /** The radius of the sphere */
    Material* mat; /** The material of the sphere */
};
}
