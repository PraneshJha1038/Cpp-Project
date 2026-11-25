#include "world/Room.hpp"

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

std::string directionToString(Direction dir) {
    switch (dir) {
        case Direction::North: return "north";
        case Direction::South: return "south";
        case Direction::East:  return "east";
        case Direction::West:  return "west";
    }
    return "unknown";
}

std::optional<Direction> parseDirection(const std::string& text) {
    const std::string lower = toLower(text);
    if (lower == "n" || lower == "north") return Direction::North;
    if (lower == "s" || lower == "south") return Direction::South;
    if (lower == "e" || lower == "east")  return Direction::East;
    if (lower == "w" || lower == "west")  return Direction::West;
    return std::nullopt;
}

Direction oppositeDirection(Direction dir) noexcept {
    switch (dir) {
        case Direction::North: return Direction::South;
        case Direction::South: return Direction::North;
        case Direction::East:  return Direction::West;
        case Direction::West:  return Direction::East;
    }
    return Direction::North;
}

Room::Room(std::string name, std::string description)
    : name_(std::move(name)),
      description_(std::move(description)) {}

void Room::setExit(Direction dir, Room* target) {
    if (target) {
        exits_[dir] = target;
    }
}

Room* Room::getExit(Direction dir) const noexcept {
    auto it = exits_.find(dir);
    return (it != exits_.end()) ? it->second : nullptr;
}

void Room::addItem(std::unique_ptr<Item> item) {
    if (item) {
        items_.push_back(std::move(item));
    }
}

std::unique_ptr<Item> Room::takeItem(const std::string& query) {
    if (query.empty()) {
        return nullptr;
    }
    const std::string needle = toLower(query);
    for (auto it = items_.begin(); it != items_.end(); ++it) {
        if (toLower((*it)->getName()).find(needle) != std::string::npos) {
            std::unique_ptr<Item> found = std::move(*it);
            items_.erase(it);
            return found;
        }
    }
    return nullptr;
}

void Room::addEnemy(std::unique_ptr<Enemy> enemy) {
    if (enemy) {
        enemies_.push_back(std::move(enemy));
    }
}

Enemy* Room::findAliveEnemy(const std::string& query) const {
    const std::string needle = toLower(query);
    for (const auto& enemy : enemies_) {
        if (!enemy->isAlive()) {
            continue;
        }
        if (needle.empty() || toLower(enemy->getName()).find(needle) != std::string::npos) {
            return enemy.get();
        }
    }
    return nullptr;
}

std::vector<Enemy*> Room::getAliveEnemies() const {
    std::vector<Enemy*> alive;
    for (const auto& enemy : enemies_) {
        if (enemy->isAlive()) {
            alive.push_back(enemy.get());
        }
    }
    return alive;
}

bool Room::hasAliveEnemies() const noexcept {
    return std::any_of(enemies_.begin(), enemies_.end(), [](const std::unique_ptr<Enemy>& e) {
        return e->isAlive();
    });
}

void Room::pruneDefeatedEnemies() {
    enemies_.erase(
        std::remove_if(enemies_.begin(), enemies_.end(), [](const std::unique_ptr<Enemy>& e) {
            return !e->isAlive();
        }),
        enemies_.end());
}

std::string Room::describe() const {
    std::ostringstream oss;
    oss << "\n--- " << name_ << " ---\n";
    oss << description_ << "\n";

    oss << "Exits: ";
    if (exits_.empty()) {
        oss << "None";
    } else {
        bool first = true;
        for (const auto& [dir, target] : exits_) {
            if (!first) oss << ", ";
            oss << directionToString(dir) << " (" << target->getName() << ")";
            first = false;
        }
    }
    oss << "\n";

    if (!items_.empty()) {
        oss << "Items on the floor:\n";
        for (const auto& item : items_) {
            oss << "  * " << item->describe() << "\n";
        }
    }

    auto alive = getAliveEnemies();
    if (!alive.empty()) {
        oss << "Hostiles present:\n";
        for (const Enemy* enemy : alive) {
            oss << "  ! " << enemy->describe() << "\n";
        }
    }

    return oss.str();
}

} // namespace shadowvault
