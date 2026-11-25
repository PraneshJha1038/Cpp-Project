#include "game/GameEngine.hpp"

#include <iostream>
#include <string>

int main() {
    std::cout << "Enter your hero's name (default: Adventurer): ";
    std::string name;
    std::getline(std::cin, name);
    if (name.empty()) {
        name = "Adventurer";
    }

    shadowvault::GameEngine engine(name);
    engine.run(std::cin, std::cout);
    return 0;
}
