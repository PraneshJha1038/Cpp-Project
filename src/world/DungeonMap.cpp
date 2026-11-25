#include "world/DungeonMap.hpp"
#include "items/Armor.hpp"
#include "items/Consumable.hpp"
#include "items/Weapon.hpp"

#include <algorithm>

namespace shadowvault {

Room* DungeonMap::createRoom(std::string name, std::string description) {
    rooms_.push_back(std::make_unique<Room>(std::move(name), std::move(description)));
    return rooms_.back().get();
}

void DungeonMap::connectBidirectional(Room* from, Direction dir, Room* to) {
    if (from && to) {
        from->setExit(dir, to);
        to->setExit(oppositeDirection(dir), from);
    }
}

Room* DungeonMap::buildDefaultDungeon() {
    rooms_.clear();

    Room* entrance = createRoom(
        "Vault Entrance",
        "Cold stone steps descend into a torch-lit foyer. Ancient runes hum faintly on the walls.");
    Room* corridor = createRoom(
        "Mossy Corridor",
        "Damp flagstones stretch onward. Shadows flicker between crumbling pillars.");
    Room* study = createRoom(
        "Alchemist's Study",
        "Shattered glass vials and dried herbs litter a heavy oak workbench.");
    Room* armory = createRoom(
        "Bone Armory",
        "Rusted weapon racks line the chamber, guarded by an undying sentinel.");
    Room* antechamber = createRoom(
        "Obsidian Antechamber",
        "Black volcanic stone radiates intense heat from the sanctum ahead.");
    Room* sanctum = createRoom(
        "Sanctum of Ash",
        "A vast vaulted cavern strewn with charred treasure and dragonfire.");

    connectBidirectional(entrance, Direction::North, corridor);
    connectBidirectional(corridor, Direction::East, study);
    connectBidirectional(corridor, Direction::West, armory);
    connectBidirectional(corridor, Direction::North, antechamber);
    connectBidirectional(antechamber, Direction::North, sanctum);

    // Entrance loot
    entrance->addItem(std::make_unique<Weapon>(
        "Rusty Shortsword", "Better than bare fists.", 15, 5, 3));
    entrance->addItem(std::make_unique<HealingPotion>(
        "Minor Healing Potion", "A small vial of crimson tonic.", 15, 35));

    // Corridor
    corridor->addEnemy(std::make_unique<Goblin>("Sneaky Goblin"));
    corridor->addItem(std::make_unique<Armor>(
        "Leather Vest", "Supple boiled leather.", 25, 3, 10));

    // Alchemist's Study
    study->addItem(std::make_unique<PowerElixir>(
        "Elixir of Might", "Glowing amber draught that sharpens your strikes.", 45, 4));
    study->addItem(std::make_unique<HealingPotion>(
        "Greater Healing Potion", "Rich ruby draught that mends deep wounds.", 35, 65));

    // Bone Armory
    armory->addEnemy(std::make_unique<SkeletonKnight>("Crypt Warden"));
    armory->addItem(std::make_unique<Weapon>(
        "Steel Longsword", "Well-balanced knight's blade.", 60, 11, 8));
    armory->addItem(std::make_unique<Armor>(
        "Chainmail Hauberk", "Interlocking steel rings.", 65, 6, 25));

    // Obsidian Antechamber
    antechamber->addEnemy(std::make_unique<Goblin>("Ash Stalker"));
    antechamber->addEnemy(std::make_unique<SkeletonKnight>("Obsidian Guard"));
    antechamber->addItem(std::make_unique<HealingPotion>(
        "Greater Healing Potion", "Rich ruby draught that mends deep wounds.", 35, 65));

    // Sanctum of Ash (Boss)
    sanctum->addEnemy(std::make_unique<ShadowDragon>("Malakor the Shadow Dragon"));
    sanctum->addItem(std::make_unique<Weapon>(
        "Runeblade of Dawn", "Radiant mythic sword humming with solar fire.", 250, 20, 15));

    return entrance;
}

bool DungeonMap::isBossDefeated() const noexcept {
    bool bossExists = false;
    for (const auto& room : rooms_) {
        for (const Enemy* enemy : room->getAliveEnemies()) {
            if (enemy->isBoss()) {
                return false;
            }
            bossExists = true;
        }
    }
    (void)bossExists;
    return true;
}

} // namespace shadowvault
