cmake_minimum_required(VERSION 3.5)

# Set the project name
project (GTaceUnitTests)

#############################################
# Unit tests

add_subdirectory(google-test)

# enable CTest testing
enable_testing()

# Add a testing executable
add_executable(GTaceUnitTests test_ref.cpp test_geometry.cpp test_math.cpp)

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
endif(MSVC)

add_test(test_all GTaceUnitTests)
