#include "items/Consumable.hpp"
#include "characters/Player.hpp"

#include <sstream>

namespace shadowvault {

HealingPotion::HealingPotion(std::string name,
                             std::string description,
                             int goldValue,
                             int healAmount)
    : Consumable(std::move(name), std::move(description), goldValue),
      healAmount_(healAmount) {}

std::string HealingPotion::describe() const {
    std::ostringstream oss;
    oss << "[Potion] " << getName() << " (Restores " << healAmount_
        << " HP, " << getGoldValue() << "g) - " << getDescription();
    return oss.str();
}

std::string HealingPotion::use(Player& player) {
    const int restored = player.heal(healAmount_);
    std::ostringstream oss;
    oss << "You drink " << getName() << " and restore " << restored << " HP"
        << " (" << player.getStats().hp << "/" << player.getStats().maxHp << " HP).";
    return oss.str();
}

PowerElixir::PowerElixir(std::string name,
                         std::string description,
                         int goldValue,
                         int attackBoost)
    : Consumable(std::move(name), std::move(description), goldValue),
      attackBoost_(attackBoost) {}

std::string PowerElixir::describe() const {
    std::ostringstream oss;
    oss << "[Elixir] " << getName() << " (+" << attackBoost_
        << " Base ATK, " << getGoldValue() << "g) - " << getDescription();
    return oss.str();
}

std::string PowerElixir::use(Player& player) {
    player.boostBaseAttack(attackBoost_);
    std::ostringstream oss;
    oss << "You consume " << getName() << "! Base ATK permanently increased by +"
        << attackBoost_ << ".";
    return oss.str();
}

} // namespace shadowvault
