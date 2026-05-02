# HelloVoxel

HelloVoxel is a modular **C++ voxel engine** built with **SDL3 GPU + Vulkan/SPIR-V**, focused on two outcomes:

1. a clean engine foundation for building voxel games in C++
2. an evolving level-editor workflow inspired by Minecraft Creative mode

<img width="400" height="225" alt="engine_demo" src="https://github.com/Linirby/HelloVoxel/blob/main/assets/demo/engine_demo.gif" />

## Why this project exists

HelloVoxel is designed as both:

- a usable base for voxel gameplay prototypes
- a reviewable engineering project with clear architecture boundaries

It prioritizes explicit systems (world, meshing, render, physics, input) over engine magic.

## Who this README is for

| Audience | What to look at first |
| --- | --- |
| Users trying the project | [Build and run](#build-and-run), [Controls](#controls) |
| Reviewers evaluating architecture | [Architecture](#architecture), [Rendering flow](#rendering-flow) |
| Contributors extending systems | [Project status](#project-status), [Next milestones](#next-milestones) |

## Current feature set

### Implemented

- SDL3 window + GPU device lifecycle with dedicated core wrappers (`src/core/*`)
- Chunked voxel world model (`16x16x16` chunks) with global block addressing
- Block registry with per-face texture atlas mapping
- Hidden-face-culling chunk mesher
- Player controller with three modes:
  - **Physical**: gravity, collision, jump, sprint
  - **Spectator**: free-fly
  - **Builder**: creative movement + voxel raycast placement/removal
- Runtime world persistence:
  - save edited map (`Ctrl+S` -> `custom_map.json`)
  - reload current resources/map (`R`)
- Split rendering path:
  - world queue/pass for 3D voxel geometry
  - UI queue/pass for sprites/HUD (crosshair)
- Directional lighting in world shading (normal-based)
- Fixed timestep simulation loop (`1/60`) with decoupled frame rendering

### In progress / scaffolded

- `src/render/passes/shadow_pass.*`
- `src/world/material.*`

These files are scaffolding and are not yet exposed as completed user-facing features.

## Architecture

The codebase is organized as layered static libraries in `CMakeLists.txt`:

| Tier | Library | Responsibility | Main paths |
| --- | --- | --- | --- |
| 1 | `LiliCore` | SDL init + window ownership | `src/core/*` |
| 1 | `LiliGeometry` | Math primitives (`Vec3`, `Mat4`) | `src/geometry/*` |
| 2 | `LiliPhysics` | Voxel collision + raycast interfaces | `src/physics/*` |
| 2 | `LiliWorld` | Blocks, chunks, map storage + map I/O | `src/world/*` |
| 3 | `LiliMeshing` | Voxel meshing + mesh primitives | `src/meshing/*` |
| 3 | `LiliEntity` | Player behavior and movement modes | `src/entity/*` |
| 4 | `LiliRender` | GPU resources, pipelines, passes, scene | `src/render/*` |
| 5 | `HelloVoxel` | App lifecycle and main loop wiring | `src/app.*`, `src/main.cpp` |

### Render module layout

```text
src/render/
├── core/        # gpu_mesh, texture, shader
├── pipelines/   # world_pipeline, ui_pipeline
├── passes/      # world_pass, ui_pass, shadow_pass (scaffold)
├── scene/       # camera, model, sprite
├── renderer.cpp
└── renderer.hpp
```

## Rendering flow

1. **Initialization**
   - Create SDL system and window
   - Create renderer, GPU device, shaders, pipelines, and render passes
   - Load map, build chunk meshes, upload GPU buffers, create models/textures
   - Create UI sprite(s), including crosshair

2. **Per-frame**
   - Poll input/events
   - Run fixed-step player/physics update
   - `begin_frame(camera)`
   - Submit world draw commands to `RenderLayer::World3D`
   - Submit UI draw commands to `RenderLayer::UI2D`
   - `end_frame()` executes world pass then UI pass and submits the command buffer

## Controls

| Input | Action |
| --- | --- |
| Mouse | Look around |
| `Tab` | Toggle relative mouse mode |
| `W` / `S` | Move forward / backward |
| `A` / `D` | Strafe left / right |
| `Space` | Jump (Physical) / Move up (Spectator, Builder) |
| `Left Shift` | Sprint forward (Physical) / Move down (Spectator, Builder) |
| `P` | Toggle Spectator mode |
| `B` | Toggle Builder mode |
| `Left Click` | Remove targeted block (Builder) |
| `Right Click` | Place block on targeted surface (Builder) |
| `Ctrl` + `S` | Save map to `custom_map.json` |
| `R` | Reload current map/resources |
| `Esc` | Exit |

## Build and run

### Requirements

- CMake 3.16+
- C++23-capable compiler
- SDL3 (`SDL3::SDL3`)
- SDL3_image (`SDL3_image::SDL3_image`)
- `glslc` (for shader compilation in `shader/`)

### Build engine binaries

```bash
sh build.sh
```

### Build shaders (required)

```bash
make -C shader
```

### Run

```bash
./build/HelloVoxel
```

Run with a specific map:

```bash
./build/HelloVoxel assets/maps/test_01.json
```

### Clean / rebuild

```bash
sh clean.sh
sh rebuild.sh
```

## Project status

HelloVoxel is currently a clean, modular voxel-engine foundation with working world interaction and rendering loops.

It is suitable for experimentation and architecture review, and is actively evolving toward stronger editor tooling and larger-world support.

## Next milestones

- Deliver a first practical in-engine level editor workflow (creative-inspired UX)
- Integrate material and shadow systems into the active render path
- Improve large-world handling (chunk streaming and related optimizations)
- Expand gameplay-oriented engine APIs for faster voxel game prototyping
