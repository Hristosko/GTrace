project(GTraceLib)

add_library(${PROJECT_NAME}
    FileUtils.cpp
)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}
)