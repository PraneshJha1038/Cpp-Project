#pragma once

#include "game/Command.hpp"

#include <memory>
#include <string>

namespace shadowvault {

/**
 * @brief Parses raw player text input into polymorphic Command instances.
 */
class CommandParser {
public:
    [[nodiscard]] std::unique_ptr<Command> parse(const std::string& input) const;
};

} // namespace shadowvault
