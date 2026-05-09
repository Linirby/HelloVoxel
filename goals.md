# HelloVoxel - Project Goals

## Vision
Build a C++ voxel engine with a level editor that feels **programmable** and **creator-friendly**, mainly through a command chat instead of a heavy traditional UI.

## Main Objectives

1. **Command-first level editing**
   - Add an in-engine chat/console where creators type commands to build and edit levels.
   - Example commands: `use <block_name>`, `set_spawn <x> <y> <z>`, `set_end <x> <y> <z>`.

2. **Extensible command system**
   - Let developers add their own commands in C++.
   - Support custom commands for custom blocks, tools, and game rules.

3. **Minimal UI, not a classic editor**
   - Keep visuals lightweight (status/help/feedback), while most actions happen through commands.
   - Prioritize speed, clarity, and low UI complexity.

4. **Physics flexibility**
   - Ship with a default physics system.
   - Allow users/projects to plug in custom physics behavior when needed.

5. **Strong engine + editor workflow**
   - Keep the architecture modular and clean.
   - Make game creation workflows simple to understand and fast to iterate on.

## Success Criteria

- A creator can open the editor and perform core tasks by command chat alone.
- Adding a new command or block workflow in C++ is straightforward.
- Projects can choose default physics or custom physics without rewriting the engine.
- The editor remains easy to use without becoming a full traditional UI stack.
