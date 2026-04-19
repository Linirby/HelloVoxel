# HelloVoxel

HelloVoxel is a C++ voxel game engine with a Vulkan backend.  
This repository is a lightweight, custom voxel game engine demonstrating a complete, low-level graphics and gameplay pipeline built entirely on **SDL3 GPU**.

<img width="400" height="225" alt="engine_demo" src="https://github.com/Linirby/HelloVoxel/blob/main/assets/demo/engine_demo.gif" />

## Table of Contents
- [Why this project exists](#why-this-project-exists)
- [Current capabilities](#current-capabilities)
- [Architecture (big picture)](#architecture-big-picture)
- [Rendering flow (from startup to frame)](#rendering-flow-from-startup-to-frame)
  - [1. Initialization](#1-initialization)
  - [2. Per-frame loop](#2-per-frame-loop)
  - [3. Shutdown](#3-shutdown)
- [Voxel meshing explained](#voxel-meshing-explained-non-graphics-friendly)
- [Shader pipeline](#shader-pipeline)
- [Controls](#controls)
- [Build and run](#build-and-run)
- [Project status](#project-status)
- [Next milestones](#next-milestones)

## Why this project exists

I built this engine to demonstrate practical, low-level graphics engineering, focusing on core implementation details:

- low-level GPU resource lifecycle
- mesh generation from voxel data
- texture atlas mapping
- camera movement and matrix math
- render pass and pipeline setup
- clean architectural abstraction between logic and rendering

## Current capabilities

- SDL3 window creation and GPU device setup.
- Vulkan-backed rendering via SDL GPU API.
- 16x16x16 chunk data model (`Chunk`) with block IDs.
- Multi-chunk `Map` support, with levels and spawn points loaded from JSON (`nlohmann/json`).
- Block registry with per-face texture selection (`BlockDefinition`).
- Chunk mesher with hidden-face culling (only visible faces become triangles).
- Indexed mesh rendering with depth buffer.
- Texture atlas sampling with nearest-neighbor filtering.
- Robust player physics including gravity, jumping, AABB voxel collision detection, and toggleable Physical/Spectator modes.
- Custom math types (`Vec3`, `Mat4`) for transforms and view/projection.
- Clean separation of the render pipeline (`Renderer`, `GPUMesh`, `Model`, `Texture`, `Shader`).
- HUD rendering with a crosshair overlay.

## Architecture (big picture)

| System | Responsibility | Main files |
| --- | --- | --- |
| App lifecycle | Init, main loop (events, updates, render), cleanup | `src/app.cpp`, `src/app.hpp`, `src/main.cpp` |
| World & Data | Map loading, chunk storage, and block registry | `src/map.*`, `src/map_loader.*`, `src/geometry/chunk.*`, `src/geometry/block.*` |
| Meshing | Convert voxels to renderable triangles | `src/geometry/mesher.*`, `src/geometry/voxel_data.hpp` |
| Render pipeline | Encapsulates SDL3 GPU setup, pipelines, and draws | `src/render/renderer.*`, `src/render/gpu_mesh.*`, `src/render/model.*`, `src/render/texture.*`, `src/render/shader.*` |
| Camera + player | Mouse orientation, physics, and AABB collision | `src/render/camera.*`, `src/entity/player.*` |
| Math | Vector and matrix operations | `src/math/vec3.*`, `src/math/mat4x4.*` |
| Shaders | Vertex transform + atlas sampling | `shader/triangle.vert`, `shader/triangle.frag` |

## Rendering flow (from startup to frame)

### 1. Initialization

`App::run()` executes this sequence:

1. `init_window()` -> creates SDL window and enables relative mouse mode.
2. Creates `Renderer` instance -> claims the window, initializes SDL GPU device, depth buffer, pipelines (including HUD), and shaders.
3. Loads a `Map` from JSON and iterates over chunks to generate `MeshData` using `ChunkMesher::generate_mesh()`.
4. Creates `GPUMesh` objects -> uploads vertices and indices to GPU buffers for each chunk.
5. Loads a `Texture` from `assets/cube_atlas.png` -> creates sampler.
6. Assembles `Model` objects linking the `GPUMesh`es and `Texture`.

### 2. Per-frame loop

In each frame:

1. Poll SDL events (quit, escape, mouse motion).
2. Update player position, handle physics/collisions, and process keyboard input.
3. Sync camera position with player.
4. `Renderer::begin_frame(camera)` -> Acquires swapchain texture, builds projection/view matrices, and begins the main render pass.
5. `Renderer::draw(model, transform)` -> Binds pipeline, vertex/index buffers, texture sampler, and draws the geometry for all active chunks in the map.
6. `Renderer::end_frame()` -> Draws the HUD/crosshair, ends the render pass, and submits the GPU command buffer.

### 3. Shutdown

`cleanup()` releases the renderer, window, and SDL subsystems in order. The `Renderer` destructor handles releasing pipelines, shaders, buffers, textures, and the GPU device.

## Voxel meshing explained (non-graphics friendly)

**Meshing** means converting a 3D grid of blocks into triangles the GPU can draw.

In this project:

- The world is organized into chunks of `16 x 16 x 16` blocks.
- Each non-air block checks 6 neighbors (top, bottom, right, left, front, back).
- If a neighbor is empty or outside the chunk, that face is visible and gets emitted.
- Each emitted face adds:
  - 4 vertices (position + UV)
  - 6 indices (2 triangles)
- UVs are computed from a texture atlas index stored in `BlockDefinition`.

Result: internal faces are skipped, so the mesh is much smaller than "draw every cube face".

## Shader pipeline

- **Vertex shader** takes position and UV, applies MVP transform.
- **Fragment shader** samples the atlas texture using UV.
- Vertex format is:
  - `location 0`: `vec3` position
  - `location 1`: `vec2` UV

SPIR-V binaries (`*.spv`) are loaded at runtime from the `shader/` folder.

## Controls

| Input | Action |
| --- | --- |
| Mouse | Look around |
| `W` / `S` | Move forward / backward |
| `A` / `D` | Strafe left / right |
| `Space` | Jump (Physical) / Move up (Spectator) |
| `Left Shift` | Move down (Spectator) |
| `P` | Toggle Physical/Spectator mode |
| `Esc` | Exit |

## Build and run

### Requirements

- CMake 3.16+
- C++ toolchain compatible with your local SDL3 packages
- SDL3 (CMake package: `SDL3::SDL3`)
- SDL3_image (CMake package: `SDL3_image::SDL3_image`)

### Build

```bash
sh build.sh
```

or manually:

```bash
cmake -B build
cmake --build build
```

### Run

```bash
./build/HelloVoxel
```

### Clean

```bash
sh clean.sh
```

### Rebuild

```bash
sh rebuild.sh
```

or:

```bash
sh clean.sh && sh build.sh
```

## Project status

This is an actively growing foundation.  
Right now it focuses on fundations for a voxel engine (multi-chunk map, textured faces, active physics and collision, camera + controls).

## Next milestones

- Level editor in free cam.
- Dynamic lighting and shading pipeline.
- Chunk streaming optimization for large worlds.
