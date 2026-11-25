#include "game/CommandParser.hpp"

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

std::string trim(const std::string& s) {
    const auto start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    const auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

} // namespace

std::unique_ptr<Command> CommandParser::parse(const std::string& input) const {
    const std::string cleaned = trim(input);
    if (cleaned.empty()) {
        return nullptr;
    }

    std::istringstream iss(cleaned);
    std::string verb;
    iss >> verb;
    verb = toLower(verb);

    std::string rest;
    std::getline(iss, rest);
    rest = trim(rest);

    if (auto directDir = parseDirection(verb)) {
        return std::make_unique<MoveCommand>(*directDir);
    }

    if (verb == "go" || verb == "move" || verb == "walk") {
        if (auto dir = parseDirection(rest)) {
            return std::make_unique<MoveCommand>(*dir);
        }
        return nullptr;
    }

    if (verb == "look" || verb == "l") {
        return std::make_unique<LookCommand>();
    }
    if (verb == "take" || verb == "get" || verb == "pickup") {
        return std::make_unique<TakeCommand>(rest);
    }
    if (verb == "use" || verb == "equip" || verb == "drink" || verb == "consume") {
        return std::make_unique<UseCommand>(rest);
    }
    if (verb == "attack" || verb == "hit" || verb == "fight" || verb == "kill") {
        return std::make_unique<AttackCommand>(rest);
    }
    if (verb == "inventory" || verb == "inv" || verb == "i" || verb == "bag") {
        return std::make_unique<InventoryCommand>();
    }
    if (verb == "stats" || verb == "status" || verb == "me") {
        return std::make_unique<StatsCommand>();
    }
    if (verb == "help" || verb == "?") {
        return std::make_unique<HelpCommand>();
    }
    if (verb == "quit" || verb == "exit" || verb == "q") {
        return std::make_unique<QuitCommand>();
    }

    return nullptr;
}

} // namespace shadowvault
