#pragma once
#include "geometry/BVH.h"
#include "camera/Camera.h"

namespace gtrace
{
struct SceneSettings
{
    uint64_t width;
    uint64_t height;
};

struct Scene
{
    SceneSettings sceneSettings;
    std::unique_ptr<Camera> camera;
    BVH geometry;
};

}  // namespace gtrace
