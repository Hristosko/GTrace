#include "TestUtils.h"

#include "geometry/BVH.h"
#include "geometry/Sphere.h"

using namespace gtrace;

class TestShape : public Shape
{
public:
    static int hitCallsCount;

    static ParsedParams getParams(float radius, const Vector3f& position)
    {
        ParsedParams params;
        params.addFloat("radius", radius);
        params.addTranslation(position);
        return params;
    }

    TestShape(float radius, const Vector3f& position) : sphere(getParams(radius, position)) {}

    bool hit(const Ray& ray, float tmin, float tmax, float time, Intersection* intersection) const override
    {
        ++hitCallsCount;
        return sphere.hit(ray, tmin, tmax, time, intersection);
    }
    BBox bound() const override { return sphere.bound(); }

private:
    Sphere sphere;
};

int TestShape::hitCallsCount = 0;

class BVHTest : public Test
{
public:
    void SetUp() override
    {
        TestShape::hitCallsCount = 0;

        std::vector<std::unique_ptr<Shape>> shapes;
        std::vector<Vector3f> positions = {
            Vector3f(1.f, 1.f, 1.f),  Vector3f(1.f, 1.f, -1.f),  Vector3f(1.f, -1.f, 1.f),  Vector3f(1.f, -1.f, -1.f),
            Vector3f(-1.f, 1.f, 1.f), Vector3f(-1.f, 1.f, -1.f), Vector3f(-1.f, -1.f, 1.f), Vector3f(-1.f, -1.f, -1.f),
        };

        shapes.reserve(positions.size());
        for (const auto& p : positions)
            shapes.emplace_back(new TestShape(R, p));

        bvh = BVH::build(shapes.data(), shapes.size());
    }

    static constexpr float R = 2.f;
    std::unique_ptr<Shape> bvh;
};

TEST_F(BVHTest, intersect_axis)
{
    const Ray ray(Vector3f(R, R, R * 10.f), Vector3f(0.f, 0.f, -1.f));
    Intersection intersection;
    bvh->hit(ray, 0.f, 1000.f, 0.f, &intersection);
    ASSERT_EQ(2, TestShape::hitCallsCount);
}
