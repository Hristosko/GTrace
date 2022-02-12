cmake_minimum_required(VERSION 3.5)
project(GTraceLib)

file(GLOB files
    "*.cpp"
    "camera/*.cpp"
    "geometry/*.cpp"
    "integrator/*.cpp"
    "light/*.cpp"
    "material/*.cpp"
    "math/*.cpp"
	"perf.*.cpp"
    "reflection/*.cpp"
    "renderer/*.cpp"
    "scene/*.cpp"
    "texture/*.cpp"
)

add_library(${PROJECT_NAME}
	${files}
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
else()
    if(NOT CMAKE_BUILD_TYPE)
      set(CMAKE_BUILD_TYPE Release)
    endif()

    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter -Wno-sized-deallocation -march=native -std=c++17")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
endif()


target_compile_definitions(${PROJECT_NAME} PUBLIC GTRACE_MEMORY_BENCH)

if(NOT MSVC)
    target_link_libraries(${PROJECT_NAME} pthread stdc++)
endif()
