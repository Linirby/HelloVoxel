# HelloVoxel

HelloVoxel is my long-term portfolio project: a C++ voxel engine focused on
**creator-friendly building** and practical workflows for making games.

<img width="400" height="225" alt="engine_demo" src="https://github.com/Linirby/HelloVoxel/blob/main/assets/demo/demo_screenshot.png" />

## Vision

Build a voxel engine + editor that feels **programmable** and **easy to use**.

The direction is command-first: creators should be able to build and configure
levels through an in-engine chat/console, without relying on a heavy traditional
editor UI.

## What you can do today

- Explore a 3D voxel world
- Switch movement modes:
  - **Physical**: gravity, collision, jump, sprint
  - **Spectator**: free-fly movement
  - **Builder**: creative movement + place/remove blocks
- Cycle the selected block with the mouse wheel (Builder)
- See an on-screen FPS counter
- See the selected block ID HUD (Builder)
- Save your edited map (`Ctrl+S` -> `custom_map.json`)
- Reload current resources/map (`R`)

## Quick start

### Requirements

- CMake 3.16+
- C++23-capable compiler
- SDL3 (`SDL3::SDL3`)
- SDL3_image (`SDL3_image::SDL3_image`)
- `glslc` (optional for users, needed when shaders must be recompiled)

### 1) Build the engine

```bash
sh build.sh
```

### 2) Build shaders (only if missing or outdated)

If `.spv` shader files are already committed and up to date, you can skip this
step.

```bash
make -C shader
```

### 3) Run

```bash
./build/HelloVoxel
```

Run with a specific map:

```bash
./build/HelloVoxel assets/maps/test_01.json
```

## Using the API (code-first)

HelloVoxel is code-first: the engine is exposed as C++ modules you compose.
`src/app.cpp` is a reference wiring for core pieces like:

- `Window`, `Renderer`, `WorldRuntime`
- `BlockRegistry` / `MaterialRegistry`
- Input wrappers (`Event`, `Keyboard`, `Mouse`) plus `Player` and `Camera`

If you want to embed or extend the engine, start from that wiring and replace
the example logic with your own gameplay or tools. `src/main.cpp` shows how the
example app is launched with an optional map path.

Clean / rebuild:

```bash
sh clean.sh
sh rebuild.sh
```

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
| Mouse Wheel | Cycle selected block (Builder) |
| `Ctrl` + `S` | Save map to `custom_map.json` |
| `R` | Reload current map/resources |
| `Esc` | Exit |

## Goals to achieve

1. **Command-first level editing**
   - Add an in-engine chat/console for editing workflows (`use`, `set_spawn`, `set_end`, etc.).
2. **Extensible command system**
   - Keep it simple for developers to add custom C++ commands.
3. **Minimal UI**
   - Focus on clear status/help/feedback instead of a large editor interface.
4. **Physics flexibility**
   - Support both default physics and project-specific custom behavior.
5. **Strong engine + editor workflow**
   - Keep architecture modular and iteration fast.

## Project status

I plan to add optimization work (frustum culling, etc.) next.
I also plan to let projects provide and link their own Player into the engine.

## Technical snapshot

Under the hood, HelloVoxel uses SDL3 GPU + Vulkan/SPIR-V shaders with a modular
C++ architecture (world, meshing, render, entity, physics). Rendering currently
uses a directional light with vertex ambient occlusion.
