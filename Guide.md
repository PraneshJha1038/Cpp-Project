# ShadowVault Player Handbook (`Guide.md`)

Welcome to **ShadowVault**, a turn-based text dungeon crawler where you explore interconnected chambers, gather gear, level up, and battle the Shadow Dragon **Malakor**.

---

## 1. Building & Launching the App

### Prerequisites
- A C++20-compatible compiler (`g++` 10+ or `clang++` 12+)
- `CMake` 3.20 or newer

### Build Commands
```bash
cmake -S . -B build
cmake --build build -j
```

### Start Playing
```bash
./build/shadowvault
```

### Run Unit Tests
```bash
ctest --test-dir build --output-on-failure
```

---

## 2. Starting Your Adventure

1. When you launch `./build/shadowvault`, you are prompted:
   ```text
   Enter your hero's name (default: Adventurer):
   ```
   Type a name and press **Enter** (or just press **Enter** to use `Adventurer`).
2. You begin in the **Vault Entrance** at **Level 1** with:
   - **120 / 120 HP**
   - **14 Base ATK**, **3 Base DEF**, **12% CRIT**
   - **25 Gold**
3. The prompt displays your current status on every turn:
   ```text
   [Adventurer | HP: 120/120 | Lv.1] >
   ```

---

## 3. Command Reference

Commands are **case-insensitive**, and item/enemy names support **partial matching** (e.g., `take sword` matches `Rusty Shortsword`).

| Command | Aliases | Turn Consumed? | Description & Example |
| :--- | :--- | :--- | :--- |
| `look` | `l` | No | Inspect the current room, exits, floor items, and living enemies. |
| `go <direction>` | `move`, `walk`, or direct `n`, `s`, `e`, `w`, `north`, `south`, `east`, `west` | No | Move to an adjacent room. Example: `go north` or `n`. |
| `take <item>` | `get`, `pickup` | No | Pick up an item from the room floor into your inventory. Example: `take potion`. |
| `use <item>` | `equip`, `drink`, `consume` | Yes (if valid) | Drink a potion/elixir (consumed) or equip a weapon/armor (swaps previous gear back to inventory). Example: `equip shortsword`. |
| `attack [enemy]` | `hit`, `fight`, `kill` | Yes (if target exists) | Attack a living enemy in the room. If `[enemy]` is omitted, targets the first living enemy. Example: `attack goblin`. |
| `inventory` | `inv`, `i`, `bag` | No | List all items currently in your bag. |
| `stats` | `status`, `me` | No | Display your full character sheet (HP, ATK, DEF, CRIT, XP, Gold, Equipment). |
| `help` | `?` | No | Display the in-game command summary. |
| `quit` | `exit`, `q` | No | Exit the game. |

> **Tip on Turns:** Commands that consume a turn (`attack` and valid `use`/`equip`) allow any living enemies in your current room to take their turn immediately afterward. Non-combat commands (`look`, `stats`, `inventory`, `help`, `go`) do **not** trigger enemy counterattacks.

---

## 4. Game Mechanics

### Combat & Damage
- **Attack Roll**: Damage is based on your **Effective ATK** (`Base ATK + Equipped Weapon ATK`) with a small random variance (`-2` to `+2`).
- **Critical Hits**: Each attack rolls against your **Effective CRIT%** (`Base CRIT + Weapon CRIT`, capped at `100%`). A critical hit deals **1.75x** raw damage.
- **Defense Mitigation**: Incoming damage is reduced by the defender's **Effective DEF** (`Base DEF + Equipped Armor DEF`), with a minimum of **1 damage** dealt if raw damage is positive.

### Items & Equipment
- **Weapons**: Boost **ATK** and optionally **CRIT%**. Equipping a new weapon automatically returns your previously equipped weapon to your inventory.
- **Armor**: Boosts **DEF** and **Max HP**. Equipping armor increases both your maximum and current HP by its Max HP bonus; swapping armor adjusts Max HP accordingly.
- **Consumables**:
  - **Healing Potions** restore HP (up to your Max HP) and are removed from your inventory after use.
  - **Elixirs** (e.g., *Elixir of Might*) permanently boost your **Base ATK** and are removed after use.

### Leveling Up
Defeating enemies grants **XP** and **Gold**. When your XP reaches the threshold (starting at `100 XP`, scaling by `1.5x` per level), you automatically level up:
- **+20 Max HP** and a **Full Heal**
- **+3 Base ATK**
- **+1 Base DEF**

---

## 5. The Dungeon & Enemies

ShadowVault consists of 6 interconnected chambers:

```text
                [Sanctum of Ash] (Boss: Malakor)
                       |
                       N
                       |
               [Obsidian Antechamber]
                       |
                       N
                       |
[Bone Armory] -- W -- [Mossy Corridor] -- E -- [Alchemist's Study]
                       |
                       S
                       |
                [Vault Entrance] (Start)
```

### Enemies You Will Face
- **Goblin** (`Crypt Goblin`, `Sneaky Goblin`, `Ash Stalker`): Nimble attacker with a 30% chance to perform a double-strike (*Flurry of Daggers*).
- **Skeleton Knight** (`Crypt Warden`, `Obsidian Guard`): Heavily armored undead warrior that can regenerate 8 HP with its cursed shield.
- **Shadow Dragon** (`Malakor the Shadow Dragon` — **Boss**): Guards the *Sanctum of Ash*. Every 3rd turn, unleashes **Shadow Flame Breath** for heavy damage.

---

## 6. Win & Loss Conditions

- **Victory**: Slay **Malakor the Shadow Dragon** in the *Sanctum of Ash*.
- **Defeat**: Your HP drops to `0`.

### Recommended Strategy
1. Pick up and equip the `Rusty Shortsword` and grab the `Minor Healing Potion` in the **Vault Entrance** before heading north.
2. Explore the **Alchemist's Study** (east of the corridor) and **Bone Armory** (west of the corridor) to upgrade your weapon, armor, and base attack before challenging the **Obsidian Antechamber** and **Sanctum of Ash**.
