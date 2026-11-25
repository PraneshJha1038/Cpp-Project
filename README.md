# ShadowVault

An interactive, turn-based terminal dungeon crawler written in **C++20**, designed to showcase clean Object-Oriented Programming (OOP) principles, RAII resource management, and design patterns.

## OOP Architecture & Design

- **Abstraction & Encapsulation**:
  - `Entity` (`include/core/Entity.hpp`) is the abstract base class for all named world objects, encapsulating unique IDs, names, and descriptions.
  - `Stats` (`include/core/Stats.hpp`) encapsulates vitality and combat math (`hp`, `maxHp`, `attack`, `defense`, `critChance`).
- **Inheritance & Runtime Polymorphism**:
  - **Items (`include/items/`)**: `Item` -> `Weapon`, `Armor`, and `Consumable` -> `HealingPotion`, `PowerElixir`. Each overrides `use(Player&)` and `describe()`.
  - **Characters (`include/characters/`)**: `Character` -> `Player` and `Enemy` -> `Goblin`, `SkeletonKnight`, `ShadowDragon`. Each enemy implements its own polymorphic `takeTurn(Player&, std::mt19937&)` AI and `getBattleCry()`.
- **Command Pattern (`include/game/`)**:
  - `Command` interface with concrete commands (`LookCommand`, `MoveCommand`, `TakeCommand`, `UseCommand`, `AttackCommand`, `InventoryCommand`, `StatsCommand`, `HelpCommand`, `QuitCommand`) created by `CommandParser`.
- **Ownership & Modern C++**:
  - `std::unique_ptr` manages polymorphic objects in `Room`, `DungeonMap`, `Player`, and `CommandParser`.

## Build & Run

```bash
cmake -S . -B build
cmake --build build -j
```

### Run the Interactive Game
```bash
./build/shadowvault
```

### Run Unit Tests
```bash
ctest --test-dir build --output-on-failure
```

## Gameplay Commands

- `look` (`l`) — Inspect the current room, exits, items, and enemies
- `go <dir>` or `n` / `s` / `e` / `w` — Move between chambers
- `take <item>` — Pick up an item from the floor
- `use <item>` / `equip <item>` — Consume a potion/elixir or equip a weapon/armor
- `attack [enemy]` — Strike an enemy in combat
- `inventory` (`inv`) — List carried items
- `stats` — View your character sheet
- `help` (`?`) — Show command reference
- `quit` (`q`) — Exit the game
