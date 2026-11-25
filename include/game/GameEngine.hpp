#pragma once

#include "characters/Player.hpp"
#include "game/CommandParser.hpp"
#include "world/DungeonMap.hpp"
#include "world/Room.hpp"

#include <istream>
#include <memory>
#include <ostream>
#include <random>
#include <string>

namespace shadowvault {

/**
 * @brief Coordinates the game loop, combat turns, world state, and player interaction.
 */
class GameEngine {
public:
    explicit GameEngine(std::string playerName = "Adventurer",
                        unsigned int seed = std::random_device{}());

    [[nodiscard]] Player& getPlayer() noexcept { return *player_; }
    [[nodiscard]] const Player& getPlayer() const noexcept { return *player_; }
    [[nodiscard]] Room* getCurrentRoom() const noexcept { return currentRoom_; }
    [[nodiscard]] std::mt19937& getRng() noexcept { return rng_; }
    [[nodiscard]] bool isRunning() const noexcept { return running_; }
    [[nodiscard]] bool isVictory() const noexcept { return victory_; }

    void setCurrentRoom(Room* room) noexcept { currentRoom_ = room; }
    void stop() noexcept { running_ = false; }

    /**
     * @brief Processes a single line of user input and returns the game response.
     */
    std::string processInput(const std::string& line);

    /**
     * @brief Runs the interactive REPL until quit, victory, or defeat.
     */
    void run(std::istream& in, std::ostream& out);

private:
    std::string runEnemyTurns();

    DungeonMap map_;
    std::unique_ptr<Player> player_;
    Room* currentRoom_{nullptr};
    CommandParser parser_;
    std::mt19937 rng_;
    bool running_{true};
    bool victory_{false};
};

} // namespace shadowvault
