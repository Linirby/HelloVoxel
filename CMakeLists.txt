cmake_minimum_required(VERSION 3.16)
project(HelloVoxel)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
	src/geometry/chunk.cpp
	src/geometry/mesher.cpp
	src/geometry/primitives.cpp
)
target_include_directories(LiliGeometry PUBLIC src)
target_link_libraries(LiliGeometry PRIVATE LiliMath)

# Lili Render
add_library(LiliRender STATIC
	src/render/gpu_mesh.cpp
	src/render/camera.cpp
	src/render/renderer.cpp
	src/render/shader.cpp
	src/render/texture.cpp
	src/render/model.cpp
)
target_include_directories(LiliRender PUBLIC src)
target_link_libraries(LiliRender PRIVATE LiliGeometry)

# Lili Entity
add_library(LiliEntity STATIC
	src/entity/player.cpp
	src/entity/playerv2.cpp
)
target_include_directories(LiliEntity PUBLIC src)
target_link_libraries(LiliRender PRIVATE LiliMath)

# Main App
add_executable(${PROJECT_NAME}
	src/main.cpp
	src/app.cpp
	src/map.cpp
	src/map_loader.cpp
)
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} PRIVATE
	LiliRender
	LiliEntity
	SDL3::SDL3
	SDL3_image::SDL3_image
)
