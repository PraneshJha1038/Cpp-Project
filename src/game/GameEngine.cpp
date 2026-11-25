#include "game/GameEngine.hpp"

#include <sstream>

namespace shadowvault {

GameEngine::GameEngine(std::string playerName, unsigned int seed)
    : player_(std::make_unique<Player>(std::move(playerName))),
      rng_(seed) {
    currentRoom_ = map_.buildDefaultDungeon();
    if (currentRoom_) {
        currentRoom_->markVisited();
    }
}

std::string GameEngine::runEnemyTurns() {
    if (!currentRoom_ || !currentRoom_->hasAliveEnemies()) {
        return "";
    }

    std::ostringstream oss;
    for (Enemy* enemy : currentRoom_->getAliveEnemies()) {
        if (!player_->isAlive()) {
            break;
        }
        oss << "\n" << enemy->takeTurn(*player_, rng_);
    }
    return oss.str();
}

std::string GameEngine::processInput(const std::string& line) {
    auto cmd = parser_.parse(line);
    if (!cmd) {
        return "Unknown command. Type 'help' to see available commands.";
    }

    CommandResult result = cmd->execute(*this);
    std::ostringstream oss;
    oss << result.output;

    if (running_ && result.endTurn && player_->isAlive()) {
        oss << runEnemyTurns();
    }

    if (map_.isBossDefeated() && !victory_) {
        victory_ = true;
        running_ = false;
        oss << "\n\n============================================\n"
            << "  VICTORY! You have slain Malakor and conquered the ShadowVault!\n"
            << "============================================";
    } else if (!player_->isAlive()) {
        running_ = false;
        oss << "\n\n============================================\n"
            << "  YOU HAVE FALLEN IN THE SHADOWVAULT...\n"
            << "============================================";
    }

    return oss.str();
}

void GameEngine::run(std::istream& in, std::ostream& out) {
    out << "============================================\n"
        << "        WELCOME TO SHADOWVAULT (C++20)      \n"
        << "============================================\n"
        << "Type 'help' for a list of commands.\n"
        << currentRoom_->describe() << "\n";

    std::string line;
    while (running_) {
        out << "\n[" << player_->getName()
            << " | HP: " << player_->getStats().hp << "/" << player_->getStats().maxHp
            << " | Lv." << player_->getLevel() << "] > " << std::flush;

        if (!std::getline(in, line)) {
            break;
        }

        const std::string response = processInput(line);
        if (!response.empty()) {
            out << response << "\n";
        }
    }
}

} // namespace shadowvault
