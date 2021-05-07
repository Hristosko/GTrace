project(GTaceConsole)

add_executable(${PROJECT_NAME} ConsoleApp.cpp)

target_link_libraries( ${PROJECT_NAME}
    sub::gtrace
)