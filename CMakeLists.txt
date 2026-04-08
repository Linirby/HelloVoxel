cmake_minimum_required(VERSION 3.16)
project(HelloVoxel)

find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)

# Lili Math
add_library(LiliMath STATIC
    src/math/mat4x4.cpp
    src/math/vec3.cpp
)
target_include_directories(LiliMath PUBLIC src)

# Lili Geometry
add_library(LiliGeometry STATIC
	src/geometry/block.cpp
	src/geometry/camera.cpp
	src/geometry/chunk.cpp
    src/geometry/mesher.cpp
	src/geometry/player.cpp
)
target_include_directories(LiliGeometry PUBLIC src)
target_link_libraries(LiliGeometry PRIVATE LiliMath)

# Main App
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/app.cpp
    src/utils.cpp
)
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    LiliGeometry
    SDL3::SDL3
    SDL3_image::SDL3_image
)
