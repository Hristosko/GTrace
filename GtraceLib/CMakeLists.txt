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

target_compile_definitions(${PROJECT_NAME}
    # should work for gcc and clang, not supported for msvc
    PUBLIC cxx_std_17
)

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)

target_compile_definitions(${PROJECT_NAME} PUBLIC GTRACE_MEMORY_BENCH)