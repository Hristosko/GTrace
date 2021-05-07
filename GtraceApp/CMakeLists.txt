project(GTaceApp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
set(SRCS Events.cpp GTraceApp.cpp)

add_executable(${PROJECT_NAME} WIN32 ${SRCS})

target_link_libraries( ${PROJECT_NAME}
    sub::gtrace
)
