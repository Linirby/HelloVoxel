# HelloVoxel

HelloVoxel is a C++ voxel rendering prototype built on **SDL3 GPU** (Vulkan backend).  
This repository demonstrates the complete pipeline from voxel data on CPU memory to textured pixels on screen, without relying on a full game engine.

## Why this project exists

I built this engine to demonstrate practical, low-level graphics engineering, focusing on core implementation details::

- low-level GPU resource lifecycle
- mesh generation from voxel data
- texture atlas mapping
- camera movement and matrix math
- render pass and pipeline setup

## Current capabilities

- SDL3 window creation and GPU device setup.
- Vulkan-backed rendering via SDL GPU API.
- 16x16x16 chunk data model (`Chunk`) with block IDs.
- Block registry with per-face texture selection (`BlockDefinition`).
- Chunk mesher with hidden-face culling (only visible faces become triangles).
- Indexed mesh rendering with depth buffer.
- Texture atlas sampling with nearest-neighbor filtering.
- Mouse-look camera + fly movement controls.
- Custom math types (`Vec3`, `Mat4`) for transforms and view/projection.

## Architecture (big picture)

| System | Responsibility | Main files |
| --- | --- | --- |
| App lifecycle | Init, main loop (events, updates, render), cleanup | `src/app.cpp`, `src/app.hpp`, `src/main.cpp` |
| Voxel data | Chunk storage and block registry | `src/geometry/chunk.*`, `src/geometry/block.*` |
| Meshing | Convert voxels to renderable triangles | `src/geometry/mesher.*`, `src/geometry/voxel_data.hpp` |
| Camera + player | Mouse orientation + keyboard movement | `src/geometry/camera.*`, `src/geometry/player.*` |
| Math | Vector and matrix operations | `src/math/vec3.*`, `src/math/mat4x4.*` |
| GPU helpers | Shader loading and buffer/texture uploads | `src/utils.*` |
| Shaders | Vertex transform + atlas sampling | `shader/triangle.vert`, `shader/triangle.frag` |

## Rendering flow (from startup to frame)

### 1. Initialization

`App::run()` executes this sequence:

1. `init_window()` -> creates SDL window and enables relative mouse mode.
2. `init_device()` -> creates SDL GPU device (`vulkan`) and claims the window.
3. `init_textures()` -> creates depth texture, loads `assets/cube_atlas.png`, creates sampler.
4. `init_test_chunk()` -> generates a test chunk layout in memory.
5. `init_buffers()` -> generates chunk mesh, uploads vertices/indices to GPU buffers.
6. `init_shaders()` -> loads SPIR-V shader binaries.
7. `init_graphics_pipeline()` -> creates raster, depth, and vertex input pipeline state.

### 2. Per-frame loop

In each frame:

1. Poll SDL events (quit, escape, mouse motion).
2. Update player position from keyboard input.
3. Sync camera position with player.
4. Build `model`, `view`, `projection`, then `MVP`.
5. Acquire swapchain texture.
6. Begin render pass, bind pipeline + buffers + texture sampler.
7. Draw indexed chunk geometry.
8. Submit GPU command buffer.

### 3. Shutdown

`cleanup()` releases pipeline/shaders, buffers, textures, sampler, GPU device, window, and SDL subsystems in order.

## Voxel meshing explained (non-graphics friendly)

**Meshing** means converting a 3D grid of blocks into triangles the GPU can draw.

In this project:

- The world piece is a single `Chunk` (`16 x 16 x 16` blocks).
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
| `Space` | Move up |
| `Left Shift` | Move down |
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
Right now it focuses on correctness for a voxel renderer core (single chunk, textured faces, camera + controls, full SDL GPU path).

## Next milestones

- Cleaner Architecture of the render pipeline.
- Multi-chunk world generation and management.
- Meshing optimizations (for fewer triangles and faster updates).
- Better lighting/shading pipeline.
- More robust camera/player physics and collision.
