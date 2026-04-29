cmake_minimum_required(VERSION 3.16)
project(HelloVoxel)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)

# ==============================================================================
#  > TIER 1
# ==============================================================================

# Lili Core
add_library(LiliCore STATIC
	src/core/sdl_system.cpp
	src/core/window.cpp
	src/core/input.cpp
)
target_include_directories(LiliCore PUBLIC src)
target_link_libraries(LiliCore PRIVATE SDL3::SDL3)

# Lili Math
add_library(LiliMath STATIC
	src/math/mat4x4.cpp
	src/math/vec3.cpp
)
target_include_directories(LiliMath PUBLIC src)

# ==============================================================================
#  > TIER 2
# ==============================================================================

# Lili Physics
add_library(LiliPhysics STATIC
	src/physics/aabb.cpp
	src/physics/raycast.cpp
)
target_include_directories(LiliPhysics PUBLIC src)
target_link_libraries(LiliPhysics PRIVATE LiliMath)

# Lili World
add_library(LiliWorld STATIC
	src/world/block.cpp
	src/world/chunk.cpp
	src/world/map.cpp
	src/world/map_manager.cpp
)
target_include_directories(LiliWorld PUBLIC src)
target_link_libraries(LiliWorld PRIVATE LiliMath)

# ==============================================================================
#  > TIER 3
# ==============================================================================

# Lili Meshing
add_library(LiliMeshing STATIC
	src/meshing/mesher.cpp
	src/meshing/primitives.cpp
)
target_include_directories(LiliMeshing PUBLIC src)
target_link_libraries(LiliMeshing PRIVATE LiliWorld)

# Lili Entity
add_library(LiliEntity STATIC
	src/entity/player.cpp
)
target_include_directories(LiliEntity PUBLIC src)
target_link_libraries(LiliEntity PRIVATE LiliMath LiliPhysics LiliWorld)

# ==============================================================================
#  > TIER 4
# ==============================================================================

# Lili Render
add_library(LiliRender STATIC
	src/render/core/gpu_mesh.cpp
	src/render/core/shader.cpp
	src/render/core/texture.cpp
	src/render/passes/renderer.cpp
	src/render/passes/shadow_pass.cpp
	src/render/passes/forward_pass.cpp
	src/render/scene/camera.cpp
	src/render/scene/model.cpp
)
target_include_directories(LiliRender PUBLIC src)
target_link_libraries(LiliRender PRIVATE
	LiliMath
	LiliMeshing
	SDL3_image::SDL3_image
)

# ==============================================================================
#  > TIER 5
# ==============================================================================

# Main App
add_executable(${PROJECT_NAME}
	src/main.cpp
	src/app.cpp
)
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} PRIVATE
	LiliCore
	LiliWorld
	LiliMeshing
	LiliEntity
	LiliRender
	SDL3::SDL3
)
