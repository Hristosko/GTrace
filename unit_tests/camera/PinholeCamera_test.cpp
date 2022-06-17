#include "TestUtils.h"
#include "camera/PinholeCamera.h"
#include "common/Errors.h"
#include "common/Random.h"
#include "parser/ParamNames.h"

using namespace gtrace;

class PinholeCameraTest : public Test
{
public:
    static ParsedParams defaultParams()
    {
        ParsedParams params;
        params.addVector3f(CAMERA_EYE, Vector3f(0.f, -100.f, 0.f));
        params.addVector3f(CAMERA_LOOK_POINT, Vector3f(0.f));
        params.addVector3f(CAMERA_UP, Vector3f(0.f, 0.f, 1.f));
        params.addFloat(CAMERA_VIEW_PLANE_DISTANCE, 10.f);
        params.addFloat(CAMERA_ZOOM, 1.f);

        return params;
    }

    RandomGenerator rg;
    std::unique_ptr<PinholeCamera> camera;
};

TEST_F(PinholeCameraTest, centerRay)
{
    const auto params = defaultParams();
    camera = std::make_unique<PinholeCamera>(params);

    const Ray ray = camera->castRay(0.f, 0.f);
    EXPECT_EQ(params.getVector3f(CAMERA_EYE), ray.origin);
    EXPECT_EQ(Vector3f(0.f, 1.f, 0.f), ray.direction);
}

TEST_F(PinholeCameraTest, castRayForPixel)
{
    const auto params = defaultParams();
    camera = std::make_unique<PinholeCamera>(params);

    const float x = 2.f;
    const float y = 2.f;
    const Ray ray = camera->castRay(x, y);
    EXPECT_EQ(params.getVector3f(CAMERA_EYE), ray.origin);
    ASSERT_NEAR(1.f, ray.direction.lengthSqr(), EPS);

    const auto expectedDirection = normalize(Vector3f(x, params.getFloat(CAMERA_VIEW_PLANE_DISTANCE), y));
    EXPECT_EQ(expectedDirection, ray.direction);
}

TEST_F(PinholeCameraTest, castRayNegativePixelCoordinate)
{
    const auto params = defaultParams();
    camera = std::make_unique<PinholeCamera>(params);

    const float x = 2.f;
    const float y = -2.f;
    const Ray ray = camera->castRay(x, y);
    EXPECT_EQ(params.getVector3f(CAMERA_EYE), ray.origin);
    ASSERT_NEAR(1.f, ray.direction.lengthSqr(), EPS);

    const auto expectedDirection = normalize(Vector3f(x, params.getFloat(CAMERA_VIEW_PLANE_DISTANCE), y));
    EXPECT_EQ(expectedDirection, ray.direction);
}

TEST_F(PinholeCameraTest, castRayRandomPixelCoordinate)
{
    const auto params = defaultParams();
    camera = std::make_unique<PinholeCamera>(params);

    const float x = rg.get();
    const float y = rg.get();
    const Ray ray = camera->castRay(x, y);
    EXPECT_EQ(params.getVector3f(CAMERA_EYE), ray.origin);
    ASSERT_NEAR(1.f, ray.direction.lengthSqr(), EPS);

    const auto expectedDirection = normalize(Vector3f(x, params.getFloat(CAMERA_VIEW_PLANE_DISTANCE), y));
    EXPECT_EQ(expectedDirection, ray.direction);
}

TEST_F(PinholeCameraTest, flipUp)
{
    auto params = defaultParams();
    params.addVector3f(CAMERA_UP, -params.getVector3f(CAMERA_UP));
    camera = std::make_unique<PinholeCamera>(params);

    const float x = rg.get();
    const float y = rg.get();
    const Ray ray = camera->castRay(x, y);
    EXPECT_EQ(params.getVector3f(CAMERA_EYE), ray.origin);
    ASSERT_NEAR(1.f, ray.direction.lengthSqr(), EPS);

    const auto expectedDirection = normalize(Vector3f(-x, params.getFloat(CAMERA_VIEW_PLANE_DISTANCE), -y));
    EXPECT_EQ(expectedDirection, ray.direction);
}

TEST_F(PinholeCameraTest, zoom)
{
    auto params = defaultParams();
    const float k = 2.f;
    params.addFloat(CAMERA_ZOOM, k);
    camera = std::make_unique<PinholeCamera>(params);

    const float x = rg.get();
    const float y = rg.get();
    const Ray ray = camera->castRay(x, y);
    EXPECT_EQ(params.getVector3f(CAMERA_EYE), ray.origin);
    ASSERT_NEAR(1.f, ray.direction.lengthSqr(), EPS);

    const auto expectedDirection = normalize(Vector3f(k * x, params.getFloat(CAMERA_VIEW_PLANE_DISTANCE), k * y));
    EXPECT_EQ(expectedDirection, ray.direction);
}

TEST_F(PinholeCameraTest, stroll)
{
    auto params = defaultParams();
    const float k = 0.2f;
    params.addFloat(CAMERA_ZOOM, k);
    camera = std::make_unique<PinholeCamera>(params);

    const float x = rg.get();
    const float y = rg.get();
    const Ray ray = camera->castRay(x, y);
    EXPECT_EQ(params.getVector3f(CAMERA_EYE), ray.origin);
    ASSERT_NEAR(1.f, ray.direction.lengthSqr(), EPS);

    const auto expectedDirection = normalize(Vector3f(k * x, params.getFloat(CAMERA_VIEW_PLANE_DISTANCE), k * y));
    EXPECT_EQ(expectedDirection, ray.direction);
}

TEST_F(PinholeCameraTest, viewPlaneDistance)
{
    auto params = defaultParams();
    const float k = 0.2f;
    params.addFloat(CAMERA_VIEW_PLANE_DISTANCE, k);
    camera = std::make_unique<PinholeCamera>(params);

    const float x = rg.get();
    const float y = rg.get();
    const Ray ray = camera->castRay(x, y);
    EXPECT_EQ(params.getVector3f(CAMERA_EYE), ray.origin);
    ASSERT_NEAR(1.f, ray.direction.lengthSqr(), EPS);

    const auto expectedDirection = normalize(Vector3f(x, params.getFloat(CAMERA_VIEW_PLANE_DISTANCE), y));
    EXPECT_EQ(expectedDirection, ray.direction);
}

TEST_F(PinholeCameraTest, lookPoint)
{
    auto params = defaultParams();
    params.addVector3f(CAMERA_LOOK_POINT, Vector3f(0.f, -200.f, 0.f));
    camera = std::make_unique<PinholeCamera>(params);

    const float x = rg.get();
    const float y = rg.get();
    const Ray ray = camera->castRay(x, y);
    EXPECT_EQ(params.getVector3f(CAMERA_EYE), ray.origin);
    ASSERT_NEAR(1.f, ray.direction.lengthSqr(), EPS);

    const auto expectedDirection = normalize(Vector3f(-x, -params.getFloat(CAMERA_VIEW_PLANE_DISTANCE), y));
    EXPECT_EQ(expectedDirection, ray.direction);
}

TEST_F(PinholeCameraTest, invalidZoom)
{
    auto params = defaultParams();
    params.addFloat(CAMERA_ZOOM, -1.f);
    EXPECT_THROW(std::make_unique<PinholeCamera>(params), InvalidValue);
}

TEST_F(PinholeCameraTest, InvalidViewPlaneDistance)
{
    auto params = defaultParams();
    params.addFloat(CAMERA_VIEW_PLANE_DISTANCE, -1.f);
    EXPECT_THROW(std::make_unique<PinholeCamera>(params), InvalidValue);
}
