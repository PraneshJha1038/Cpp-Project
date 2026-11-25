# ShadowVault Architecture & File Workflow (`workflow.md`)

This document explains the exact runtime execution flow of **ShadowVault** and describes the purpose of every file in the repository.

---

## 1. Exact Runtime Workflow

### Step 1: Startup & Initialization (`src/main.cpp` -> `GameEngine`)
1. `main()` reads the hero's name from `std::cin` (defaulting to `"Adventurer"`).
2. `GameEngine` is constructed:
   - Creates a `Player` instance (`std::make_unique<Player>`).
   - Seeds the Mersenne Twister RNG (`std::mt19937`).
   - Calls `DungeonMap::buildDefaultDungeon()`, which instantiates all 6 `Room`s, links their bidirectional exits, and populates each room with `Item`s and `Enemy` subclasses.
   - Sets `currentRoom_` to the starting room (`Vault Entrance`) and marks it as visited.

### Step 2: Interactive REPL Loop (`GameEngine::run`)
1. Prints the welcome banner and `currentRoom_->describe()`.
2. Enters `while (running_)`:
   - Prints the status prompt: `[Name | HP: cur/max | Lv.N] > `.
   - Reads a line from `std::istream`.
   - Passes the line to `GameEngine::processInput(line)`.

### Step 3: Input Parsing (`CommandParser::parse`)
1. `CommandParser::parse(line)` trims whitespace, extracts the first word as the lowercase `verb`, and captures the remainder as `rest`.
2. Maps the `verb` (or direct direction shortcut `n`/`s`/`e`/`w`) to a concrete `std::unique_ptr<Command>` subclass:
   - `LookCommand`, `MoveCommand`, `TakeCommand`, `UseCommand`, `AttackCommand`, `InventoryCommand`, `StatsCommand`, `HelpCommand`, or `QuitCommand`.
   - Returns `nullptr` if unrecognized.

### Step 4: Command Execution (`Command::execute`)
1. `GameEngine::processInput` invokes `cmd->execute(*this)`, receiving a `CommandResult{output, endTurn}`.
   - **`MoveCommand`**: Looks up the target `Room*` via `Room::getExit(dir)`, updates `currentRoom_`, describes the room, and prints enemy battle cries on first visit.
   - **`TakeCommand`**: Extracts the matching `std::unique_ptr<Item>` from `Room::takeItem()` and transfers ownership to `Player::addItem()`.
   - **`UseCommand`**: Calls `Player::useItem(target)`, which invokes `Item::use(*this)` polymorphically:
     - `Weapon::use` -> `Player::equipWeaponById` (swaps into `equippedWeapon_`).
     - `Armor::use` -> `Player::equipArmorById` (swaps into `equippedArmor_` and adjusts `maxHp`).
     - `HealingPotion::use` / `PowerElixir::use` -> modifies `Player` stats and is erased from `inventory_`.
   - **`AttackCommand`**: Finds a living `Enemy*` in `currentRoom_`, calls `Player::attackTarget(*enemy, rng_)`, and if the enemy dies, awards XP/Gold, checks level-up (`Player::gainXp`), and prunes defeated enemies (`Room::pruneDefeatedEnemies`).

### Step 5: Enemy AI Turn & End-of-Turn Checks (`GameEngine::runEnemyTurns`)
1. If `running_` is true, `result.endTurn` is true, and `player_->isAlive()`:
   - Loops over all living `Enemy*` in `currentRoom_`.
   - Calls `enemy->takeTurn(*player_, rng_)` polymorphically (`Goblin` flurry, `SkeletonKnight` shield heal, `ShadowDragon` flame breath).
2. Checks win/loss conditions:
   - If `DungeonMap::isBossDefeated()` is true -> sets `victory_ = true`, `running_ = false`, and appends the victory banner.
   - Else if `!player_->isAlive()` -> sets `running_ = false` and appends the defeat banner.

---

## 2. File-by-File Reference

### Root & Build Configuration
| File | Purpose |
| :--- | :--- |
| [`.gitignore`](.gitignore) | Excludes build artifacts (`build/`, `*.o`, `*.a`, IDE folders) from git tracking. |
| [`CMakeLists.txt`](CMakeLists.txt) | Configures C++20 compilation (`-Wall -Wextra -Wpedantic`), builds `shadowvault_lib` static library, `shadowvault` executable, and `shadowvault_tests` with CTest integration. |
| [`README.md`](README.md) | Project landing page summarizing OOP design, quick-start commands, and links to docs. |
| [`Guide.md`](Guide.md) | User-facing player handbook explaining commands, combat math, items, and dungeon layout. |
| [`workflow.md`](workflow.md) | This file: documents runtime execution flow and every file in the repository. |

### Core Module (`include/core/`, `src/core/`)
| File | Purpose |
| :--- | :--- |
| [`include/core/Stats.hpp`](include/core/Stats.hpp) | Defines the `Stats` struct (`hp`, `maxHp`, `attack`, `defense`, `critChance`) with `isAlive()`, `heal()`, and `applyDamage()`. |
| [`include/core/Entity.hpp`](include/core/Entity.hpp) | Abstract base class for all named entities (`Player`, `Enemy`, `Item`) with unique ID generation and pure virtual `describe()` / `getCategory()`. |
| [`src/core/Entity.cpp`](src/core/Entity.cpp) | Implements `Entity` constructor and thread-safe atomic ID counter (`Entity::nextId()`). |

### Items Module (`include/items/`, `src/items/`)
| File | Purpose |
| :--- | :--- |
| [`include/items/Item.hpp`](include/items/Item.hpp) | Abstract base class deriving from `Entity`; defines `ItemType`, gold value, `isConsumable()`, and pure virtual `use(Player&)`. |
| [`include/items/Weapon.hpp`](include/items/Weapon.hpp) | Declares `Weapon` (`final` subclass of `Item`) with `attackBonus_` and `critBonus_`. |
| [`src/items/Weapon.cpp`](src/items/Weapon.cpp) | Implements `Weapon::describe()` and `Weapon::use()` (delegates to `Player::equipWeaponById`). |
| [`include/items/Armor.hpp`](include/items/Armor.hpp) | Declares `Armor` (`final` subclass of `Item`) with `defenseBonus_` and `maxHpBonus_`. |
| [`src/items/Armor.cpp`](src/items/Armor.cpp) | Implements `Armor::describe()` and `Armor::use()` (delegates to `Player::equipArmorById`). |
| [`include/items/Consumable.hpp`](include/items/Consumable.hpp) | Declares abstract `Consumable` base class and concrete `HealingPotion` and `PowerElixir` classes. |
| [`src/items/Consumable.cpp`](src/items/Consumable.cpp) | Implements `HealingPotion::use()` (restores HP) and `PowerElixir::use()` (permanently boosts base ATK). |

### Characters Module (`include/characters/`, `src/characters/`)
| File | Purpose |
| :--- | :--- |
| [`include/characters/Character.hpp`](include/characters/Character.hpp) | Abstract combatant deriving from `Entity`; defines `AttackResult`, virtual stat getters, `takeDamage()`, and `attackTarget()`. |
| [`src/characters/Character.cpp`](src/characters/Character.cpp) | Implements base combat math (critical hit roll, damage variance, defense mitigation). |
| [`include/characters/Player.hpp`](include/characters/Player.hpp) | Declares `Player` (`final` subclass of `Character`) managing `inventory_`, `equippedWeapon_`, `equippedArmor_`, XP, level, and gold. |
| [`src/characters/Player.cpp`](src/characters/Player.cpp) | Implements equipment swapping, item usage, stat bonuses, and `gainXp()` leveling logic. |
| [`include/characters/Enemy.hpp`](include/characters/Enemy.hpp) | Declares abstract `Enemy` base class (`takeTurn()`, `getBattleCry()`, `isBoss()`) and concrete subclasses `Goblin`, `SkeletonKnight`, and `ShadowDragon`. |
| [`src/characters/Enemy.cpp`](src/characters/Enemy.cpp) | Implements distinct AI behaviors and battle cries for `Goblin`, `SkeletonKnight`, and `ShadowDragon`. |

### World Module (`include/world/`, `src/world/`)
| File | Purpose |
| :--- | :--- |
| [`include/world/Room.hpp`](include/world/Room.hpp) | Declares `Direction` enum, direction helpers, and `Room` class (exits map, owned items, owned enemies). |
| [`src/world/Room.cpp`](src/world/Room.cpp) | Implements direction parsing, room item/enemy lookup, dead-enemy pruning, and `Room::describe()`. |
| [`include/world/DungeonMap.hpp`](include/world/DungeonMap.hpp) | Declares `DungeonMap` class that owns all `Room` instances and tracks boss defeat state. |
| [`src/world/DungeonMap.cpp`](src/world/DungeonMap.cpp) | Builds the 6-room ShadowVault dungeon graph and populates rooms with enemies and loot. |

### Game & Entry Point Module (`include/game/`, `src/game/`, `src/main.cpp`)
| File | Purpose |
| :--- | :--- |
| [`include/game/Command.hpp`](include/game/Command.hpp) | Declares `CommandResult`, abstract `Command` interface, and all 9 concrete command classes. |
| [`src/game/Command.cpp`](src/game/Command.cpp) | Implements `execute()` for `LookCommand`, `MoveCommand`, `TakeCommand`, `UseCommand`, `AttackCommand`, `InventoryCommand`, `StatsCommand`, `HelpCommand`, and `QuitCommand`. |
| [`include/game/CommandParser.hpp`](include/game/CommandParser.hpp) | Declares `CommandParser` to convert raw text input into `std::unique_ptr<Command>`. |
| [`src/game/CommandParser.cpp`](src/game/CommandParser.cpp) | Implements verb/alias normalization and command instantiation. |
| [`include/game/GameEngine.hpp`](include/game/GameEngine.hpp) | Declares `GameEngine` orchestrating `DungeonMap`, `Player`, `CommandParser`, RNG, and turn loop. |
| [`src/game/GameEngine.cpp`](src/game/GameEngine.cpp) | Implements `processInput()`, `runEnemyTurns()`, victory/defeat detection, and the interactive `run()` loop. |
| [`src/main.cpp`](src/main.cpp) | Application entry point; prompts for hero name and starts `GameEngine::run(std::cin, std::cout)`. |

### Tests (`tests/`)
| File | Purpose |
| :--- | :--- |
| [`tests/test_main.cpp`](tests/test_main.cpp) | Standalone unit test suite verifying polymorphic items/equipment, polymorphic enemy AI, and `GameEngine` navigation & combat. |
