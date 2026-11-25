#include "characters/Enemy.hpp"
#include "characters/Player.hpp"

#include <sstream>

namespace shadowvault {

Enemy::Enemy(std::string name,
             std::string description,
             Stats stats,
             int xpReward,
             int goldReward)
    : Character(std::move(name), std::move(description), stats),
      xpReward_(xpReward),
      goldReward_(goldReward) {}

// --- Goblin ---

Goblin::Goblin(std::string name)
    : Enemy(std::move(name),
            "A wiry green cave-dweller clutching a jagged dagger.",
            Stats{42, 42, 10, 1, 20},
            35,
            18) {}

std::string Goblin::describe() const {
    std::ostringstream oss;
    oss << getName() << " [HP: " << stats_.hp << "/" << stats_.maxHp
        << ", ATK: " << stats_.attack << "] - " << getDescription();
    return oss.str();
}

std::string Goblin::getBattleCry() const {
    return "\"Shiny things belong to us!\"";
}

std::string Goblin::takeTurn(Player& player, std::mt19937& rng) {
    std::uniform_int_distribution<int> roll(1, 100);
    if (roll(rng) <= 30) {
        // Nimble double-strike
        const int first = player.takeDamage(stats_.attack - 2);
        const int second = player.takeDamage(stats_.attack - 2);
        std::ostringstream oss;
        oss << getName() << " lunges with a Flurry of Daggers for "
            << first << " + " << second << " damage!";
        return oss.str();
    }
    return attackTarget(player, rng).message;
}

// --- SkeletonKnight ---

SkeletonKnight::SkeletonKnight(std::string name)
    : Enemy(std::move(name),
            "An ancient sentinel clad in rusted plate armor.",
            Stats{75, 75, 14, 5, 10},
            65,
            35) {}

std::string SkeletonKnight::describe() const {
    std::ostringstream oss;
    oss << getName() << " [HP: " << stats_.hp << "/" << stats_.maxHp
        << ", ATK: " << stats_.attack << ", DEF: " << stats_.defense
        << "] - " << getDescription();
    return oss.str();
}

std::string SkeletonKnight::getBattleCry() const {
    return "\"None shall pass the Vault's threshold.\"";
}

std::string SkeletonKnight::takeTurn(Player& player, std::mt19937& rng) {
    std::uniform_int_distribution<int> roll(1, 100);
    if (roll(rng) <= 25 && stats_.hp < stats_.maxHp) {
        const int healed = heal(8);
        std::ostringstream oss;
        oss << getName() << " raises its cursed shield, regenerating "
            << healed << " HP!";
        return oss.str();
    }
    return attackTarget(player, rng).message;
}

// --- ShadowDragon ---

ShadowDragon::ShadowDragon(std::string name)
    : Enemy(std::move(name),
            "A colossal wyrm wreathed in violet flame and obsidian scales.",
            Stats{175, 175, 22, 6, 15},
            250,
            200) {}

std::string ShadowDragon::describe() const {
    std::ostringstream oss;
    oss << "[BOSS] " << getName() << " [HP: " << stats_.hp << "/" << stats_.maxHp
        << ", ATK: " << stats_.attack << ", DEF: " << stats_.defense
        << "] - " << getDescription();
    return oss.str();
}

std::string ShadowDragon::getBattleCry() const {
    return "\"Mortal, your light ends in ash!\"";
}

std::string ShadowDragon::takeTurn(Player& player, std::mt19937& rng) {
    ++turnCounter_;
    if (turnCounter_ % 3 == 0) {
        // Every 3rd turn: Shadow Flame Breath (high raw damage)
        const int dealt = player.takeDamage(stats_.attack + 12);
        std::ostringstream oss;
        oss << getName() << " unleashes SHADOW FLAME BREATH, scorching you for "
            << dealt << " damage!";
        return oss.str();
    }
    return attackTarget(player, rng).message;
}

} // namespace shadowvault
