#include "characters/Enemy.hpp"
#include "characters/Player.hpp"
#include "game/GameEngine.hpp"
#include "items/Armor.hpp"
#include "items/Consumable.hpp"
#include "items/Weapon.hpp"
#include "world/DungeonMap.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <vector>

using namespace shadowvault;

void testPolymorphicItemsAndEquipment() {
    Player hero("TestHero");
    const int baseAtk = hero.getEffectiveAttack();
    const int baseDef = hero.getEffectiveDefense();
    const int baseMaxHp = hero.getStats().maxHp;

    hero.addItem(std::make_unique<Weapon>("Iron Blade", "Sharp blade.", 20, 7, 5));
    hero.addItem(std::make_unique<Weapon>("Steel Axe", "Heavy axe.", 40, 12, 0));
    hero.addItem(std::make_unique<Armor>("Plate Vest", "Sturdy plate.", 35, 5, 15));
    hero.addItem(std::make_unique<HealingPotion>("Red Potion", "Heals 30.", 15, 30));
    hero.addItem(std::make_unique<PowerElixir>("Might Elixir", "+3 ATK.", 30, 3));

    assert(hero.getInventory().size() == 5);

    // Equip first weapon
    hero.useItem("Iron Blade");
    assert(hero.getEquippedWeapon() != nullptr);
    assert(hero.getEquippedWeapon()->getName() == "Iron Blade");
    assert(hero.getEffectiveAttack() == baseAtk + 7);
    assert(hero.getInventory().size() == 4);

    // Swap weapon: Iron Blade should return to inventory
    hero.useItem("Steel Axe");
    assert(hero.getEquippedWeapon()->getName() == "Steel Axe");
    assert(hero.getEffectiveAttack() == baseAtk + 12);
    assert(hero.getInventory().size() == 4);

    // Equip armor: Defense and MaxHP increase
    hero.useItem("Plate Vest");
    assert(hero.getEffectiveDefense() == baseDef + 5);
    assert(hero.getStats().maxHp == baseMaxHp + 15);
    assert(hero.getInventory().size() == 3);

    // Take damage and heal with potion (consumable removed from inventory)
    hero.takeDamage(40);
    const int damagedHp = hero.getStats().hp;
    assert(damagedHp < hero.getStats().maxHp);
    hero.useItem("Red Potion");
    assert(hero.getStats().hp > damagedHp);
    assert(hero.getInventory().size() == 2);

    // Use PowerElixir
    hero.useItem("Might Elixir");
    assert(hero.getEffectiveAttack() == baseAtk + 12 + 3);
    assert(hero.getInventory().size() == 1);
}

void testPolymorphicEnemies() {
    Player hero("Defender");
    std::mt19937 rng(42);

    std::vector<std::unique_ptr<Enemy>> enemies;
    enemies.push_back(std::make_unique<Goblin>());
    enemies.push_back(std::make_unique<SkeletonKnight>());
    enemies.push_back(std::make_unique<ShadowDragon>());

    assert(!enemies[0]->isBoss());
    assert(!enemies[1]->isBoss());
    assert(enemies[2]->isBoss());

    for (auto& enemy : enemies) {
        const std::string cry = enemy->getBattleCry();
        const std::string desc = enemy->describe();
        const std::string turnMsg = enemy->takeTurn(hero, rng);
        assert(!cry.empty());
        assert(!desc.empty());
        assert(!turnMsg.empty());
    }
    assert(hero.getStats().hp < hero.getStats().maxHp);
}

void testDungeonNavigationAndEngine() {
    GameEngine engine("Tester", 12345);
    assert(engine.isRunning());
    assert(engine.getCurrentRoom()->getName() == "Vault Entrance");

    // Pick up and equip Rusty Shortsword in entrance
    const std::string takeRes = engine.processInput("take shortsword");
    assert(takeRes.find("Picked up Rusty Shortsword") != std::string::npos);

    const std::string equipRes = engine.processInput("equip shortsword");
    assert(equipRes.find("Equipped Rusty Shortsword") != std::string::npos);

    // Move north to Mossy Corridor
    const std::string moveRes = engine.processInput("n");
    assert(engine.getCurrentRoom()->getName() == "Mossy Corridor");
    assert(moveRes.find("Sneaky Goblin") != std::string::npos);

    // Attack Goblin until defeated
    while (engine.getCurrentRoom()->hasAliveEnemies() && engine.isRunning()) {
        engine.processInput("attack goblin");
    }
    assert(!engine.getCurrentRoom()->hasAliveEnemies());
    assert(engine.getPlayer().getGold() > 25);

    // Quit cleanly
    engine.processInput("quit");
    assert(!engine.isRunning());
}

int main() {
    testPolymorphicItemsAndEquipment();
    testPolymorphicEnemies();
    testDungeonNavigationAndEngine();
    std::cout << "All ShadowVault unit tests passed!\n";
    return 0;
}
