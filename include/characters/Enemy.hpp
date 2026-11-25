#pragma once

#include "characters/Character.hpp"

#include <random>
#include <string>

namespace shadowvault {

class Player;

/**
 * @brief Abstract base class for hostile creatures with polymorphic AI turns.
 */
class Enemy : public Character {
public:
    ~Enemy() override = default;

    [[nodiscard]] std::string getCategory() const override { return "Enemy"; }

    [[nodiscard]] int getXpReward() const noexcept { return xpReward_; }
    [[nodiscard]] int getGoldReward() const noexcept { return goldReward_; }
    [[nodiscard]] virtual bool isBoss() const noexcept { return false; }

    [[nodiscard]] virtual std::string getBattleCry() const = 0;

    /**
     * @brief Executes the enemy's polymorphic turn against the player.
     */
    virtual std::string takeTurn(Player& player, std::mt19937& rng) = 0;

protected:
    Enemy(std::string name,
          std::string description,
          Stats stats,
          int xpReward,
          int goldReward);

private:
    int xpReward_;
    int goldReward_;
};

/**
 * @brief Nimble enemy with a chance for a quick double-strike.
 */
class Goblin final : public Enemy {
public:
    explicit Goblin(std::string name = "Crypt Goblin");

    [[nodiscard]] std::string describe() const override;
    [[nodiscard]] std::string getBattleCry() const override;
    std::string takeTurn(Player& player, std::mt19937& rng) override;
};

/**
 * @brief Heavily armored undead warrior.
 */
class SkeletonKnight final : public Enemy {
public:
    explicit SkeletonKnight(std::string name = "Skeleton Knight");

    [[nodiscard]] std::string describe() const override;
    [[nodiscard]] std::string getBattleCry() const override;
    std::string takeTurn(Player& player, std::mt19937& rng) override;
};

/**
 * @brief Dungeon boss that alternates between claw attacks and devastating fire breath.
 */
class ShadowDragon final : public Enemy {
public:
    explicit ShadowDragon(std::string name = "Malakor the Shadow Dragon");

    [[nodiscard]] bool isBoss() const noexcept override { return true; }
    [[nodiscard]] std::string describe() const override;
    [[nodiscard]] std::string getBattleCry() const override;
    std::string takeTurn(Player& player, std::mt19937& rng) override;

private:
    int turnCounter_{0};
};

} // namespace shadowvault
