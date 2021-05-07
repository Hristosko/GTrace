project(GTaceConsole)

add_executable(${PROJECT_NAME} ConsoleApp.cpp)

target_link_libraries( ${PROJECT_NAME}
    sub::gtrace
)

target_compile_definitions(${PROJECT_NAME}
    # should work for gcc and clang, not supported for msvc
    PUBLIC cxx_std_17
)

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)