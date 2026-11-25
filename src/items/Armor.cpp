#include "items/Armor.hpp"
#include "characters/Player.hpp"

#include <sstream>

namespace shadowvault {

Armor::Armor(std::string name,
             std::string description,
             int goldValue,
             int defenseBonus,
             int maxHpBonus)
    : Item(std::move(name), std::move(description), goldValue),
      defenseBonus_(defenseBonus),
      maxHpBonus_(maxHpBonus) {}

std::string Armor::describe() const {
    std::ostringstream oss;
    oss << "[Armor] " << getName() << " (+" << defenseBonus_ << " DEF";
    if (maxHpBonus_ > 0) {
        oss << ", +" << maxHpBonus_ << " Max HP";
    }
    oss << ", " << getGoldValue() << "g) - " << getDescription();
    return oss.str();
}

std::string Armor::use(Player& player) {
    return player.equipArmorById(getId());
}

} // namespace shadowvault
