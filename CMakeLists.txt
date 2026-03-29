cmake_minimum_required(VERSION 3.10)
project(HelloVoxel)

find_package(SDL3 REQUIRED CONFIG)

file(GLOB_RECURSE SRCS "src/*.cpp")
include_directories(include/)
add_executable(${PROJECT_NAME} ${SRCS})
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
