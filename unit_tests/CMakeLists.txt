cmake_minimum_required(VERSION 3.5)

# Set the project name
project (GTaceUnitTests)

#############################################
# Unit tests

add_subdirectory(google-test)

# enable CTest testing
enable_testing()

# Add a testing executable
file(GLOB files
    "*.cpp"
    "geometry/*.cpp"
    "math/*.cpp"
    "parser/*.cpp"
)

add_executable(GTaceUnitTests ${files})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/.
)

target_link_libraries(GTaceUnitTests
    sub::gtrace
    GTest::GTest
    GTest::Main
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

    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter -march=native -std=c++17 -I../GtraceLib")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
endif()

add_test(test_all GTaceUnitTests)
