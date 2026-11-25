#include "items/Weapon.hpp"
#include "characters/Player.hpp"

#include <sstream>

namespace shadowvault {

Weapon::Weapon(std::string name,
               std::string description,
               int goldValue,
               int attackBonus,
               int critBonus)
    : Item(std::move(name), std::move(description), goldValue),
      attackBonus_(attackBonus),
      critBonus_(critBonus) {}

std::string Weapon::describe() const {
    std::ostringstream oss;
    oss << "[Weapon] " << getName() << " (+" << attackBonus_ << " ATK";
    if (critBonus_ > 0) {
        oss << ", +" << critBonus_ << "% CRIT";
    }
    oss << ", " << getGoldValue() << "g) - " << getDescription();
    return oss.str();
}

std::string Weapon::use(Player& player) {
    return player.equipWeaponById(getId());
}

} // namespace shadowvault
