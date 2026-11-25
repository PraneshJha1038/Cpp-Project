#pragma once

#include "items/Item.hpp"

namespace shadowvault {

/**
 * @brief Equippable weapon that increases attack power and critical hit chance.
 */
class Weapon final : public Item {
public:
    Weapon(std::string name,
           std::string description,
           int goldValue,
           int attackBonus,
           int critBonus = 0);

    [[nodiscard]] int getAttackBonus() const noexcept { return attackBonus_; }
    [[nodiscard]] int getCritBonus() const noexcept { return critBonus_; }

    [[nodiscard]] ItemType getItemType() const noexcept override { return ItemType::Weapon; }
    [[nodiscard]] bool isConsumable() const noexcept override { return false; }
    [[nodiscard]] std::string describe() const override;

    std::string use(Player& player) override;

private:
    int attackBonus_;
    int critBonus_;
};

} // namespace shadowvault
