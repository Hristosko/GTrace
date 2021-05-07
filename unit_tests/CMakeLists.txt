cmake_minimum_required(VERSION 3.5)

# Set the project name
project (GTaceUnitTests)

#############################################
# Unit tests

add_subdirectory(google-test)

# enable CTest testing
enable_testing()

# Add a testing executable
add_executable(GTaceUnitTests test_ref.cpp)

target_link_libraries(GTaceUnitTests
    sub::gtrace
    GTest::GTest
    GTest::Main
)

add_test(test_all GTaceUnitTests)
