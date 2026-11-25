#pragma once

#include <algorithm>

namespace shadowvault {

/**
 * @brief Encapsulates combat and vitality attributes for characters.
 */
struct Stats {
    int hp{100};
    int maxHp{100};
    int attack{10};
    int defense{2};
    int critChance{10}; // Percentage 0-100

    [[nodiscard]] bool isAlive() const noexcept {
        return hp > 0;
    }

    /**
     * @brief Restores HP up to maxHp and returns the actual amount healed.
     */
    int heal(int amount) noexcept {
        if (amount <= 0 || !isAlive()) {
            return 0;
        }
        const int previous = hp;
        hp = std::min(maxHp, hp + amount);
        return hp - previous;
    }

    /**
     * @brief Applies raw damage mitigated by defense (minimum 1 damage if raw > 0).
     * @return Actual damage dealt to HP.
     */
    int applyDamage(int rawDamage) noexcept {
        if (rawDamage <= 0 || !isAlive()) {
            return 0;
        }
        const int mitigated = std::max(1, rawDamage - defense);
        const int previous = hp;
        hp = std::max(0, hp - mitigated);
        return previous - hp;
    }
};

} // namespace shadowvault
