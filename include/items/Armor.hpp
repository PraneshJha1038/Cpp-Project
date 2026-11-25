#pragma once

#include "items/Item.hpp"

namespace shadowvault {

/**
 * @brief Equippable armor that increases defense and maximum HP.
 */
class Armor final : public Item {
public:
    Armor(std::string name,
          std::string description,
          int goldValue,
          int defenseBonus,
          int maxHpBonus = 0);

    [[nodiscard]] int getDefenseBonus() const noexcept { return defenseBonus_; }
    [[nodiscard]] int getMaxHpBonus() const noexcept { return maxHpBonus_; }

    [[nodiscard]] ItemType getItemType() const noexcept override { return ItemType::Armor; }
    [[nodiscard]] bool isConsumable() const noexcept override { return false; }
    [[nodiscard]] std::string describe() const override;

    std::string use(Player& player) override;

private:
    int defenseBonus_;
    int maxHpBonus_;
};

} // namespace shadowvault
