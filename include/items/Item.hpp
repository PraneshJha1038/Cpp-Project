#pragma once

#include "core/Entity.hpp"

#include <string>

namespace shadowvault {

class Player;

enum class ItemType {
    Weapon,
    Armor,
    Consumable
};

/**
 * @brief Abstract base class for all collectable and usable items.
 */
class Item : public Entity {
public:
    ~Item() override = default;

    [[nodiscard]] std::string getCategory() const override { return "Item"; }

    [[nodiscard]] int getGoldValue() const noexcept { return goldValue_; }
    [[nodiscard]] virtual ItemType getItemType() const noexcept = 0;
    [[nodiscard]] virtual bool isConsumable() const noexcept = 0;

    /**
     * @brief Applies or equips this item on the given player.
     * @return Human-readable message describing the result.
     */
    virtual std::string use(Player& player) = 0;

protected:
    Item(std::string name, std::string description, int goldValue)
        : Entity(std::move(name), std::move(description)),
          goldValue_(goldValue) {}

private:
    int goldValue_;
};

} // namespace shadowvault
