#pragma once

#include "core/Entity.hpp"
#include "core/Stats.hpp"

#include <random>
#include <string>

namespace shadowvault {

struct AttackResult {
    int damageDealt{0};
    bool wasCrit{false};
    std::string message;
};

/**
 * @brief Abstract base class for all combat-capable entities (Player and Enemies).
 */
class Character : public Entity {
public:
    ~Character() override = default;

    [[nodiscard]] const Stats& getStats() const noexcept { return stats_; }
    [[nodiscard]] bool isAlive() const noexcept { return stats_.isAlive(); }

    [[nodiscard]] virtual int getEffectiveAttack() const noexcept { return stats_.attack; }
    [[nodiscard]] virtual int getEffectiveDefense() const noexcept { return stats_.defense; }
    [[nodiscard]] virtual int getEffectiveCritChance() const noexcept { return stats_.critChance; }

    int heal(int amount) noexcept;
    virtual int takeDamage(int rawDamage) noexcept;

    /**
     * @brief Performs a polymorphic attack against another character.
     */
    virtual AttackResult attackTarget(Character& target, std::mt19937& rng);

protected:
    Character(std::string name, std::string description, Stats stats);

    Stats stats_;
};

} // namespace shadowvault
