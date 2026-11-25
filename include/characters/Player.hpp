#pragma once

#include "characters/Character.hpp"
#include "items/Armor.hpp"
#include "items/Item.hpp"
#include "items/Weapon.hpp"

#include <memory>
#include <string>
#include <vector>

namespace shadowvault {

/**
 * @brief Represents the user-controlled hero with inventory, equipment, and progression.
 */
class Player final : public Character {
public:
    explicit Player(std::string name);

    [[nodiscard]] std::string getCategory() const override { return "Player"; }
    [[nodiscard]] std::string describe() const override;

    [[nodiscard]] int getEffectiveAttack() const noexcept override;
    [[nodiscard]] int getEffectiveDefense() const noexcept override;
    [[nodiscard]] int getEffectiveCritChance() const noexcept override;

    [[nodiscard]] int getLevel() const noexcept { return level_; }
    [[nodiscard]] int getXp() const noexcept { return xp_; }
    [[nodiscard]] int getXpToNextLevel() const noexcept { return xpToNextLevel_; }
    [[nodiscard]] int getGold() const noexcept { return gold_; }

    [[nodiscard]] const std::vector<std::unique_ptr<Item>>& getInventory() const noexcept {
        return inventory_;
    }
    [[nodiscard]] const Weapon* getEquippedWeapon() const noexcept {
        return equippedWeapon_.get();
    }
    [[nodiscard]] const Armor* getEquippedArmor() const noexcept {
        return equippedArmor_.get();
    }

    void addItem(std::unique_ptr<Item> item);
    std::string useItem(const std::string& query);
    std::string equipWeaponById(Entity::Id id);
    std::string equipArmorById(Entity::Id id);

    void boostBaseAttack(int amount) noexcept;
    void addGold(int amount) noexcept;
    std::string gainXp(int amount);

private:
    int level_{1};
    int xp_{0};
    int xpToNextLevel_{100};
    int gold_{25};

    std::vector<std::unique_ptr<Item>> inventory_;
    std::unique_ptr<Weapon> equippedWeapon_;
    std::unique_ptr<Armor> equippedArmor_;
};

} // namespace shadowvault
