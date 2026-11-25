#include "core/Entity.hpp"

#include <atomic>
#include <utility>

namespace shadowvault {

Entity::Id Entity::nextId() noexcept {
    static std::atomic<Id> counter{1};
    return counter.fetch_add(1, std::memory_order_relaxed);
}

Entity::Entity(std::string name, std::string description)
    : id_(nextId()),
      name_(std::move(name)),
      description_(std::move(description)) {}

} // namespace shadowvault
