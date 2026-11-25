#include "characters/Player.hpp"

#include <algorithm>
#include <cctype>
#include <sstream>

namespace shadowvault {

namespace {

std::string toLower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return value;
}

} // namespace

Player::Player(std::string name)
    : Character(std::move(name),
                "A brave adventurer delving into the ShadowVault.",
                Stats{120, 120, 14, 3, 12}) {}

int Player::getEffectiveAttack() const noexcept {
    int total = stats_.attack;
    if (equippedWeapon_) {
        total += equippedWeapon_->getAttackBonus();
    }
    return total;
}

int Player::getEffectiveDefense() const noexcept {
    int total = stats_.defense;
    if (equippedArmor_) {
        total += equippedArmor_->getDefenseBonus();
    }
    return total;
}

int Player::getEffectiveCritChance() const noexcept {
    int total = stats_.critChance;
    if (equippedWeapon_) {
        total += equippedWeapon_->getCritBonus();
    }
    return std::min(100, total);
}

std::string Player::describe() const {
    std::ostringstream oss;
    oss << "=== " << getName() << " (Level " << level_ << ") ===\n"
        << "  HP:   " << stats_.hp << "/" << stats_.maxHp << "\n"
        << "  ATK:  " << getEffectiveAttack() << " (Base " << stats_.attack << ")\n"
        << "  DEF:  " << getEffectiveDefense() << " (Base " << stats_.defense << ")\n"
        << "  CRIT: " << getEffectiveCritChance() << "%\n"
        << "  XP:   " << xp_ << "/" << xpToNextLevel_ << "  |  Gold: " << gold_ << "g\n"
        << "  Weapon: " << (equippedWeapon_ ? equippedWeapon_->getName() : "None") << "\n"
        << "  Armor:  " << (equippedArmor_ ? equippedArmor_->getName() : "None");
    return oss.str();
}

void Player::addItem(std::unique_ptr<Item> item) {
    if (item) {
        inventory_.push_back(std::move(item));
    }
}

std::string Player::useItem(const std::string& query) {
    if (query.empty()) {
        return "Specify an item name to use (e.g., 'use potion').";
    }

    const std::string needle = toLower(query);
    for (std::size_t i = 0; i < inventory_.size(); ++i) {
        if (toLower(inventory_[i]->getName()).find(needle) != std::string::npos) {
            const bool consume = inventory_[i]->isConsumable();
            const Entity::Id targetId = inventory_[i]->getId();
            std::string result = inventory_[i]->use(*this);

            if (consume) {
                auto it = std::find_if(inventory_.begin(), inventory_.end(),
                                       [targetId](const std::unique_ptr<Item>& item) {
                                           return item->getId() == targetId;
                                       });
                if (it != inventory_.end()) {
                    inventory_.erase(it);
                }
            }
            return result;
        }
    }

    return "No item matching '" + query + "' found in your inventory.";
}

std::string Player::equipWeaponById(Entity::Id id) {
    auto it = std::find_if(inventory_.begin(), inventory_.end(), [id](const std::unique_ptr<Item>& item) {
        return item->getId() == id;
    });
    if (it == inventory_.end()) {
        return "Weapon not found in inventory.";
    }

    auto* rawWeapon = dynamic_cast<Weapon*>(it->get());
    if (!rawWeapon) {
        return "That item is not a weapon.";
    }

    std::unique_ptr<Item> extracted = std::move(*it);
    inventory_.erase(it);

    std::unique_ptr<Weapon> newWeapon(static_cast<Weapon*>(extracted.release()));
    std::string previousName;
    if (equippedWeapon_) {
        previousName = equippedWeapon_->getName();
        inventory_.push_back(std::move(equippedWeapon_));
    }

    equippedWeapon_ = std::move(newWeapon);

    std::ostringstream oss;
    oss << "Equipped " << equippedWeapon_->getName()
        << " (+" << equippedWeapon_->getAttackBonus() << " ATK";
    if (equippedWeapon_->getCritBonus() > 0) {
        oss << ", +" << equippedWeapon_->getCritBonus() << "% CRIT";
    }
    oss << ").";
    if (!previousName.empty()) {
        oss << " Returned " << previousName << " to inventory.";
    }
    return oss.str();
}

std::string Player::equipArmorById(Entity::Id id) {
    auto it = std::find_if(inventory_.begin(), inventory_.end(), [id](const std::unique_ptr<Item>& item) {
        return item->getId() == id;
    });
    if (it == inventory_.end()) {
        return "Armor not found in inventory.";
    }

    auto* rawArmor = dynamic_cast<Armor*>(it->get());
    if (!rawArmor) {
        return "That item is not armor.";
    }

    std::unique_ptr<Item> extracted = std::move(*it);
    inventory_.erase(it);

    std::unique_ptr<Armor> newArmor(static_cast<Armor*>(extracted.release()));
    std::string previousName;
    if (equippedArmor_) {
        previousName = equippedArmor_->getName();
        stats_.maxHp = std::max(1, stats_.maxHp - equippedArmor_->getMaxHpBonus());
        stats_.hp = std::min(stats_.hp, stats_.maxHp);
        inventory_.push_back(std::move(equippedArmor_));
    }

    equippedArmor_ = std::move(newArmor);
    stats_.maxHp += equippedArmor_->getMaxHpBonus();
    stats_.hp = std::min(stats_.maxHp, stats_.hp + equippedArmor_->getMaxHpBonus());

    std::ostringstream oss;
    oss << "Equipped " << equippedArmor_->getName()
        << " (+" << equippedArmor_->getDefenseBonus() << " DEF";
    if (equippedArmor_->getMaxHpBonus() > 0) {
        oss << ", +" << equippedArmor_->getMaxHpBonus() << " Max HP";
    }
    oss << ").";
    if (!previousName.empty()) {
        oss << " Returned " << previousName << " to inventory.";
    }
    return oss.str();
}

void Player::boostBaseAttack(int amount) noexcept {
    if (amount > 0) {
        stats_.attack += amount;
    }
}

void Player::addGold(int amount) noexcept {
    if (amount > 0) {
        gold_ += amount;
    }
}

std::string Player::gainXp(int amount) {
    if (amount <= 0) {
        return "";
    }
    xp_ += amount;
    std::ostringstream oss;
    while (xp_ >= xpToNextLevel_) {
        xp_ -= xpToNextLevel_;
        ++level_;
        xpToNextLevel_ = static_cast<int>(xpToNextLevel_ * 1.5);
        stats_.maxHp += 20;
        stats_.hp = stats_.maxHp;
        stats_.attack += 3;
        stats_.defense += 1;
        oss << "\n*** LEVEL UP! You reached Level " << level_
            << "! (+20 Max HP, Full Heal, +3 ATK, +1 DEF) ***";
    }
    return oss.str();
}

} // namespace shadowvault
