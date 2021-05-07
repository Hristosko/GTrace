project(GTraceLib)

add_library(${PROJECT_NAME}
    FileUtils.cpp
    Logger.cpp
    MemoryBench.cpp

    camera/OrthogonalCamera.cpp
    camera/PinholeCamera.cpp

    geometry/BBox.cpp
    geometry/BVH.cpp
    geometry/Mesh.cpp
    geometry/Shape.cpp
    geometry/Sphere.cpp
    geometry/Triangle.cpp

    light/DirectionalLight.cpp

    material/MatteMaterial.cpp

    math/OrthonormalBasis.cpp

    renderer/Renderer.cpp
    renderer/RendererDisplay.cpp
    renderer/RendererOutput.cpp
    renderer/RendererStat.cpp

    scene/Parser.cpp
    scene/World.cpp

    texture/NoiseTexture.cpp
    texture/SolidNoise.cpp
)

add_library(sub::gtrace ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/.
)