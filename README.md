# Echoes of Shadows

> Something is wrong with this game.

Echoes of Shadows is a story-driven terminal RPG written in C++. Explore a ruined world, uncover its history, and figure out what happened to the place you're standing in.

The game remembers.

## About

Echoes of Shadows is an experimental RPG focused on exploration, atmosphere, and discovery.

You will explore a world filled with abandoned places, strange encounters, and details that may become more important than they first appear.

The deeper you go, the more questions you may have.

## Features

- Story-driven exploration
- Turn-based RPG gameplay
- Persistent save system
- Exploration and environmental storytelling
- A world that changes as you progress
- Hidden and unusual game systems
- Original C++ implementation
- CMake-based build system

## The World

Gone.

That's the best way to describe the beauty of the castle in front of you.

All that's left is an empty shell.

The field around it seems equally lifeless.

Walking towards the ruined castle, you notice something near a stream.

A small plastic sword with tape wrapped around the hilt.

And beside it, something else.

## The Game

Echoes of Shadows is designed to let the player discover its story naturally.

The world contains details that may seem insignificant at first. Pay attention to your surroundings, remember what you've seen, and don't be afraid to investigate.

Some parts of the game become stranger as you progress.

## Project Status

**Active Development**

The game is currently under development. Story content, gameplay systems, and other features are still being expanded.

## Building

Echoes of Shadows uses CMake and can be built with a variety of compatible development environments and toolchains.

### Requirements

- C++20-compatible compiler
- CMake 3.20 or newer
- Windows, macOS, or Linux
- Git

### Recommended IDEs

- Visual Studio 2026
- CLion
- Visual Studio Code with CMake Tools
- Other CMake-compatible IDEs

### Clone and Build

```bash
git clone https://github.com/LinkZelda99-cpp/Echoes-of-Shadows.git
cd Echoes-of-Shadows

cmake -S . -B build
cmake --build build
```

The executable is named `Echoes_of_Shadows`.

### CLion

Open the repository folder in CLion. CLion will detect `CMakeLists.txt` and configure the project automatically.

For the intended terminal presentation, run the executable using an **external console** rather than CLion's built-in output window.

### Visual Studio

The repository includes the Visual Studio solution:

`Echoes of Shadows.slnx`

Open the solution in Visual Studio 2026 and build the project normally.

## Project Structure

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

The project uses C++ and CMake as its primary development tools.

CMake keeps the project independent from any single IDE, allowing development in Visual Studio, CLion, VS Code, and other compatible environments.

Build artifacts and IDE-specific files should not be committed.

## Roadmap

- Expand the story
- Add new areas and locations
- Expand RPG mechanics
- Add more encounters
- Continue developing the game's hidden systems
- Improve the terminal interface
- Expand the world and its mysteries

## Contributing

Echoes of Shadows is an open-source project.

Bug reports, ideas, improvements, and pull requests are welcome.

If you find something that appears to be a bug, feel free to open an issue.

If you want to contribute, pull requests are welcome as well.

## License

Echoes of Shadows is licensed under the MIT License.

See [LICENSE.txt](LICENSE.txt) for the full license text.

## One Last Thing

The README doesn't contain everything.

There's more to the game than what's written here.

Go play it.
