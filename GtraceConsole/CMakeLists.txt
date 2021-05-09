project(GTaceConsole)

add_executable(${PROJECT_NAME} ConsoleApp.cpp)


target_link_libraries( ${PROJECT_NAME}
    GTraceLib
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
