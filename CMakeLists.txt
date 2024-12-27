cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)

project(camera)

include(cmake/dependencies.cmake)
include(cmake/flags.cmake)

add_subdirectory(examples)

include_directories(inc)
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(${PROJECT_NAME} ${SOURCES})
add_dependencies(${PROJECT_NAME} liblccv)
add_dependencies(${PROJECT_NAME} liblogger)

target_link_libraries(${PROJECT_NAME}
    lccv
    logger
    ${OpenCV_LIBS}
)
