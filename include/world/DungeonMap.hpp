#pragma once

#include "world/Room.hpp"

#include <memory>
#include <vector>

namespace shadowvault {

/**
 * @brief Owns all Room instances and manages the dungeon graph topology.
 */
class DungeonMap {
public:
    Room* createRoom(std::string name, std::string description);
    void connectBidirectional(Room* from, Direction dir, Room* to);

    /**
     * @brief Builds the default 6-room ShadowVault dungeon and returns the starting room.
     */
    Room* buildDefaultDungeon();

    [[nodiscard]] const std::vector<std::unique_ptr<Room>>& getRooms() const noexcept {
        return rooms_;
    }

    [[nodiscard]] bool isBossDefeated() const noexcept;

private:
    std::vector<std::unique_ptr<Room>> rooms_;
};

} // namespace shadowvault
