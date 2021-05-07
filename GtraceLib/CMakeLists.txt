project(GTraceLib)

add_library(${PROJECT_NAME}
    FileUtils.cpp
)

add_library(sub::gtrace ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/.
)