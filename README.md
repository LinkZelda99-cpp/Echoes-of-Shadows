# Echoes of Shadows

A small, story-driven C++ terminal RPG prototype focused on exploration, dialogue, inventory, and an increasingly strange save system.

> **Status:** Early development

## Overview

**Echoes of Shadows** is a command-line RPG prototype written in modern C++. The project is built around a mysterious world that changes as the player explores it, with systems for:

- Exploration and world navigation
- Dialogue and typewriter-style text
- Player stats and inventory
- Save/load functionality
- Loop-based narrative changes
- A deliberately unusual save-data system

The game is currently a prototype, so mechanics and story elements are expected to change.

## Requirements

- A C++20-compatible compiler
- CMake 3.20 or newer
- Windows, macOS, or Linux

## Build with CMake

Clone the repository, then configure and build:

```bash
cmake -S . -B build
cmake --build build
```

The executable is named `Echoes_of_Shadows`.

### CLion

Open the repository folder in CLion. CLion will detect `CMakeLists.txt` and configure the project automatically.

For the intended terminal presentation, run the executable using an **external console** rather than CLion's built-in output window.

### Visual Studio

The repository also contains the Visual Studio solution:

`Echoes of Shadows.slnx`

Open the solution in Visual Studio 2026 and build the project normally.

## Project structure

| File | Purpose |
| --- | --- |
| `main.cpp` | Game entry point and main menu |
| `World.cpp/.h` | World exploration |
| `Player.cpp/.h` | Player state and inventory |
| `Battle.cpp/.h` | Battle-related systems |
| `Dialogue.cpp/.h` | Dialogue and text presentation |
| `Save.cpp/.h` | Save/load system |
| `Item.h` | Item definitions |
| `CMakeLists.txt` | Cross-platform CMake build |
| `Echoes of Shadows.slnx` | Visual Studio solution |

## Development

The project can be developed from either Visual Studio 2026 or CLion. CMake is used as the portable build configuration, while the Visual Studio solution is retained for the native Visual Studio workflow.

Build artifacts and IDE-specific files should not be committed.

## License

Echoes of Shadows is licensed under the MIT License. See [LICENSE.txt](LICENSE.txt).
