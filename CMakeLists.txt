cmake_minimum_required(VERSION 3.10)
project(HelloVoxel)

find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)

file(GLOB_RECURSE SRCS "src/*.cpp")
add_executable(${PROJECT_NAME} ${SRCS})
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
)
