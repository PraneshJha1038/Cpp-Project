#pragma once

#include "characters/Enemy.hpp"
#include "items/Item.hpp"

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace shadowvault {

enum class Direction {
    North,
    South,
    East,
    West
};

[[nodiscard]] std::string directionToString(Direction dir);
[[nodiscard]] std::optional<Direction> parseDirection(const std::string& text);
[[nodiscard]] Direction oppositeDirection(Direction dir) noexcept;

/**
 * @brief Represents a single chamber in the dungeon containing exits, items, and enemies.
 */
class Room {
public:
    Room(std::string name, std::string description);

    [[nodiscard]] const std::string& getName() const noexcept { return name_; }
    [[nodiscard]] const std::string& getDescription() const noexcept { return description_; }
    [[nodiscard]] bool isVisited() const noexcept { return visited_; }
    void markVisited() noexcept { visited_ = true; }

    void setExit(Direction dir, Room* target);
    [[nodiscard]] Room* getExit(Direction dir) const noexcept;
    [[nodiscard]] const std::map<Direction, Room*>& getExits() const noexcept { return exits_; }

    void addItem(std::unique_ptr<Item> item);
    std::unique_ptr<Item> takeItem(const std::string& query);
    [[nodiscard]] const std::vector<std::unique_ptr<Item>>& getItems() const noexcept { return items_; }

    void addEnemy(std::unique_ptr<Enemy> enemy);
    [[nodiscard]] Enemy* findAliveEnemy(const std::string& query) const;
    [[nodiscard]] std::vector<Enemy*> getAliveEnemies() const;
    [[nodiscard]] bool hasAliveEnemies() const noexcept;
    void pruneDefeatedEnemies();

    [[nodiscard]] std::string describe() const;

private:
    std::string name_;
    std::string description_;
    bool visited_{false};

    std::map<Direction, Room*> exits_;
    std::vector<std::unique_ptr<Item>> items_;
    std::vector<std::unique_ptr<Enemy>> enemies_;
};

} // namespace shadowvault
