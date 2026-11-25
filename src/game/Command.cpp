#include "game/Command.hpp"
#include "game/GameEngine.hpp"

#include <sstream>

namespace shadowvault {

CommandResult LookCommand::execute(GameEngine& engine) {
    return CommandResult{engine.getCurrentRoom()->describe(), false};
}

CommandResult MoveCommand::execute(GameEngine& engine) {
    Room* current = engine.getCurrentRoom();
    Room* next = current->getExit(direction_);
    if (!next) {
        return CommandResult{"There is no exit to the " + directionToString(direction_) + ".", false};
    }

    engine.setCurrentRoom(next);
    std::ostringstream oss;
    oss << "You move " << directionToString(direction_) << ".\n" << next->describe();

    if (!next->isVisited()) {
        next->markVisited();
        for (const Enemy* enemy : next->getAliveEnemies()) {
            oss << enemy->getName() << " snarls: " << enemy->getBattleCry() << "\n";
        }
    }

    return CommandResult{oss.str(), false};
}

CommandResult TakeCommand::execute(GameEngine& engine) {
    if (target_.empty()) {
        return CommandResult{"Take what? Specify an item name (e.g., 'take sword').", false};
    }

    auto item = engine.getCurrentRoom()->takeItem(target_);
    if (!item) {
        return CommandResult{"There is no '" + target_ + "' here.", false};
    }

    std::string name = item->getName();
    engine.getPlayer().addItem(std::move(item));
    return CommandResult{"Picked up " + name + ".", false};
}

CommandResult UseCommand::execute(GameEngine& engine) {
    std::string result = engine.getPlayer().useItem(target_);
    const bool validAction = result.find("No item matching") == std::string::npos &&
                             result.find("Specify an item") == std::string::npos;
    return CommandResult{result, validAction};
}

CommandResult AttackCommand::execute(GameEngine& engine) {
    Room* room = engine.getCurrentRoom();
    Enemy* enemy = room->findAliveEnemy(target_);
    if (!enemy) {
        if (target_.empty()) {
            return CommandResult{"There are no enemies here to attack.", false};
        }
        return CommandResult{"No living enemy matching '" + target_ + "' is here.", false};
    }

    Player& player = engine.getPlayer();
    AttackResult res = player.attackTarget(*enemy, engine.getRng());

    std::ostringstream oss;
    oss << res.message;

    if (!enemy->isAlive()) {
        oss << "\n" << enemy->getName() << " has been slain! (+"
            << enemy->getXpReward() << " XP, +" << enemy->getGoldReward() << "g)";
        player.addGold(enemy->getGoldReward());
        oss << player.gainXp(enemy->getXpReward());
        room->pruneDefeatedEnemies();
    }

    return CommandResult{oss.str(), true};
}

CommandResult InventoryCommand::execute(GameEngine& engine) {
    const Player& player = engine.getPlayer();
    const auto& inv = player.getInventory();

    std::ostringstream oss;
    oss << "=== Inventory (" << inv.size() << " items) ===\n";
    if (inv.empty()) {
        oss << "  (empty)\n";
    } else {
        for (const auto& item : inv) {
            oss << "  - " << item->describe() << "\n";
        }
    }
    return CommandResult{oss.str(), false};
}

CommandResult StatsCommand::execute(GameEngine& engine) {
    return CommandResult{engine.getPlayer().describe(), false};
}

CommandResult HelpCommand::execute(GameEngine& /*engine*/) {
    std::ostringstream oss;
    oss << "=== ShadowVault Commands ===\n"
        << "  look (l)              - Inspect your current room\n"
        << "  go <dir> / n,s,e,w    - Move north, south, east, or west\n"
        << "  take <item>           - Pick up an item from the room\n"
        << "  use <item> / equip    - Use a potion/elixir or equip weapon/armor\n"
        << "  attack [enemy]        - Attack an enemy in the room\n"
        << "  inventory (inv)       - List carried items\n"
        << "  stats                 - View your character sheet\n"
        << "  help (?)              - Show this help message\n"
        << "  quit (q)              - Exit the game";
    return CommandResult{oss.str(), false};
}

CommandResult QuitCommand::execute(GameEngine& engine) {
    engine.stop();
    return CommandResult{"You step back into the mist. Farewell, adventurer!", false};
}

} // namespace shadowvault
