project(GTaceApp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
set(SRCS Events.cpp GTraceApp.cpp)

ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

include_directories( ${PROJECT_NAME}
    ${CMAKE_BINARY_DIR}/../../../sdk/wxWidget/include/msvc
    ${CMAKE_BINARY_DIR}/../../../sdk/wxWidget/include
)

add_executable(${PROJECT_NAME} WIN32 ${SRCS})

target_link_libraries( ${PROJECT_NAME}
    sub::gtrace
)

# Add the link directories manualy
