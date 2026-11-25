#include "characters/Character.hpp"

#include <algorithm>
#include <sstream>

namespace shadowvault {

Character::Character(std::string name, std::string description, Stats stats)
    : Entity(std::move(name), std::move(description)),
      stats_(stats) {}

int Character::heal(int amount) noexcept {
    return stats_.heal(amount);
}

int Character::takeDamage(int rawDamage) noexcept {
    if (rawDamage <= 0 || !isAlive()) {
        return 0;
    }
    const int mitigated = std::max(1, rawDamage - getEffectiveDefense());
    const int previous = stats_.hp;
    stats_.hp = std::max(0, stats_.hp - mitigated);
    return previous - stats_.hp;
}

AttackResult Character::attackTarget(Character& target, std::mt19937& rng) {
    std::uniform_int_distribution<int> critRoll(1, 100);
    std::uniform_int_distribution<int> variance(-2, 2);

    const bool isCrit = critRoll(rng) <= getEffectiveCritChance();
    int rawDamage = std::max(1, getEffectiveAttack() + variance(rng));
    if (isCrit) {
        rawDamage = static_cast<int>(rawDamage * 1.75);
    }

    const int dealt = target.takeDamage(rawDamage);

    std::ostringstream oss;
    oss << getName() << " strikes " << target.getName() << " for " << dealt << " damage";
    if (isCrit) {
        oss << " (CRITICAL HIT!)";
    }
    oss << ".";

    return AttackResult{dealt, isCrit, oss.str()};
}

} // namespace shadowvault
