#pragma once

#include "items/Item.hpp"

namespace shadowvault {

/**
 * @brief Base class for single-use consumable items.
 */
class Consumable : public Item {
public:
    ~Consumable() override = default;

    [[nodiscard]] ItemType getItemType() const noexcept final { return ItemType::Consumable; }
    [[nodiscard]] bool isConsumable() const noexcept final { return true; }

protected:
    using Item::Item;
};

/**
 * @brief Restores health points when consumed.
 */
class HealingPotion final : public Consumable {
public:
    HealingPotion(std::string name, std::string description, int goldValue, int healAmount);

    [[nodiscard]] int getHealAmount() const noexcept { return healAmount_; }
    [[nodiscard]] std::string describe() const override;

    std::string use(Player& player) override;

private:
    int healAmount_;
};

/**
 * @brief Permanently increases base attack power when consumed.
 */
class PowerElixir final : public Consumable {
public:
    PowerElixir(std::string name, std::string description, int goldValue, int attackBoost);

    [[nodiscard]] int getAttackBoost() const noexcept { return attackBoost_; }
    [[nodiscard]] std::string describe() const override;

    std::string use(Player& player) override;

private:
    int attackBoost_;
};

} // namespace shadowvault
