#include "TestUtils.h"

#include "geometry/BVH.h"
#include "geometry/Sphere.h"
#include "scene/Parser.h"
#include "scene/World.h"

using namespace gtrace;

class TestShape : public Shape {
public:
    static int hitCallsCount;
    TestShape(float R, const Vector3f& pos) {
        World w;
        const SceneParser parser(w);
        std::unordered_map<std::string, std::string> settings;
        settings["radius"] = std::to_string(R);
        settings["position"] = std::string("(") +
            std::to_string(pos.x()) + ", " +
            std::to_string(pos.y()) + ", " +
            std::to_string(pos.z()) + ')';
        this->parse(parser, settings);
    }

    virtual bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const override {
        ++hitCallsCount;
        return sphere.hit(ray, tmin, tmax, time, rec);
    }
    virtual BBox bbox() const override {
        return sphere.bbox();
    }
private:
    virtual void parse(const SceneParser& parser, std::unordered_map<std::string, std::string>& map) override {
        sphere.parse(parser, map);
    }
private:
    Sphere sphere;
};

int TestShape::hitCallsCount = 0;

class BVHTest : public Test {
public:
    void SetUp() override {
        TestShape::hitCallsCount = 0;

        std::vector<std::unique_ptr<Shape>> shapes;
        std::vector<Vector3f> positions = {
            Vector3f(1.f, 1.f, 1.f),
            Vector3f(1.f, 1.f, -1.f),
            Vector3f(1.f, -1.f, 1.f),
            Vector3f(1.f, -1.f, -1.f),
            Vector3f(-1.f, 1.f, 1.f),
            Vector3f(-1.f, 1.f, -1.f),
            Vector3f(-1.f, -1.f, 1.f),
            Vector3f(-1.f, -1.f, -1.f),
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
    HitRecord rec;
    bvh->hit(ray, 0.f, 1000.f, 0.f, rec);
    ASSERT_EQ(2, TestShape::hitCallsCount);
}
