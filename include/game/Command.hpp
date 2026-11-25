#pragma once

#include "world/Room.hpp"

#include <string>

namespace shadowvault {

class GameEngine;

struct CommandResult {
    std::string output;
    bool endTurn{false};
};

/**
 * @brief Abstract Command interface (Command Pattern) for all player actions.
 */
class Command {
public:
    virtual ~Command() = default;

    [[nodiscard]] virtual CommandResult execute(GameEngine& engine) = 0;
};

class LookCommand final : public Command {
public:
    CommandResult execute(GameEngine& engine) override;
};

class MoveCommand final : public Command {
public:
    explicit MoveCommand(Direction dir) : direction_(dir) {}
    CommandResult execute(GameEngine& engine) override;

private:
    Direction direction_;
};

class TakeCommand final : public Command {
public:
    explicit TakeCommand(std::string target) : target_(std::move(target)) {}
    CommandResult execute(GameEngine& engine) override;

private:
    std::string target_;
};

class UseCommand final : public Command {
public:
    explicit UseCommand(std::string target) : target_(std::move(target)) {}
    CommandResult execute(GameEngine& engine) override;

private:
    std::string target_;
};

class AttackCommand final : public Command {
public:
    explicit AttackCommand(std::string target) : target_(std::move(target)) {}
    CommandResult execute(GameEngine& engine) override;

private:
    std::string target_;
};

class InventoryCommand final : public Command {
public:
    CommandResult execute(GameEngine& engine) override;
};

class StatsCommand final : public Command {
public:
    CommandResult execute(GameEngine& engine) override;
};

class HelpCommand final : public Command {
public:
    CommandResult execute(GameEngine& engine) override;
};

class QuitCommand final : public Command {
public:
    CommandResult execute(GameEngine& engine) override;
};

} // namespace shadowvault
