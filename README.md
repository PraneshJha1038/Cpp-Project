# ShadowVault

**ShadowVault** is an interactive, turn-based terminal dungeon crawler written in **C++20**. It is designed to demonstrate clean Object-Oriented Programming (OOP) principles, RAII memory management (`std::unique_ptr`), and classic design patterns in a playable CLI game.

## Documentation

- **[Guide.md](Guide.md)** — Complete player handbook: how to build, run, play, and master the game.
- **[workflow.md](workflow.md)** — End-to-end execution workflow and a file-by-file reference of every file in the repository.

## Key OOP Highlights

- **Abstraction & Encapsulation**: `Entity` abstract base class and `Stats` value struct (`include/core/`).
- **Polymorphic Item Hierarchy**: `Item` -> `Weapon`, `Armor`, and `Consumable` (`HealingPotion`, `PowerElixir`) (`include/items/`).
- **Polymorphic Character & Enemy AI**: `Character` -> `Player` and `Enemy` (`Goblin`, `SkeletonKnight`, `ShadowDragon`) (`include/characters/`).
- **World Graph**: `Room` and `DungeonMap` managing interconnected chambers (`include/world/`).
- **Command Pattern**: `Command`, `CommandParser`, and `GameEngine` (`include/game/`).

## Quick Start

```bash
# Configure and build
cmake -S . -B build
cmake --build build -j

# Play the game
./build/shadowvault

# Run unit tests
ctest --test-dir build --output-on-failure
```
