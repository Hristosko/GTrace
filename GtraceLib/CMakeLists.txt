cmake_minimum_required(VERSION 3.5)
project(GTraceLib)

file(GLOB files
    "camera/*.cpp"
    "common/*.cpp"
    "data_structures/*.cpp"
    "geometry/*.cpp"
    "math/*.cpp"
    "parser/*.cpp"
    "renderer/*.cpp"
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

    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-sized-deallocation -Wno-sign-compare -march=native -std=c++17")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
endif()


target_compile_definitions(${PROJECT_NAME} PUBLIC GTRACE_MEMORY_TRACKING)

if(NOT MSVC)
    target_link_libraries(${PROJECT_NAME} pthread stdc++)
endif()
