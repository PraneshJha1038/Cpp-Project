#pragma once

#include <cstdint>
#include <string>

namespace shadowvault {

/**
 * @brief Abstract base class for all named objects in the game world.
 *
 * Demonstrates abstraction, encapsulation, and polymorphic identification.
 */
class Entity {
public:
    using Id = std::uint64_t;

    virtual ~Entity() = default;

    // Non-copyable to preserve unique identity, movable by derived classes if needed.
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;
    Entity(Entity&&) noexcept = default;
    Entity& operator=(Entity&&) noexcept = default;

    [[nodiscard]] Id getId() const noexcept { return id_; }
    [[nodiscard]] const std::string& getName() const noexcept { return name_; }
    [[nodiscard]] const std::string& getDescription() const noexcept { return description_; }

    /**
     * @brief Returns a formatted description for inspection by the player.
     */
    [[nodiscard]] virtual std::string describe() const = 0;

    /**
     * @brief Returns the high-level category of this entity (e.g., "Player", "Enemy", "Item").
     */
    [[nodiscard]] virtual std::string getCategory() const = 0;

protected:
    Entity(std::string name, std::string description);

private:
    static Id nextId() noexcept;

    Id id_;
    std::string name_;
    std::string description_;
};

} // namespace shadowvault
