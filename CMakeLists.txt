cmake_minimum_required(VERSION 3.5)

project(GTrace)

add_subdirectory(GtraceLib)
add_subdirectory(GtraceConsole)
add_subdirectory(unit_tests)

if(MSVC)
    # only supported for windows for now
    add_subdirectory(GtraceApp)
endif()
