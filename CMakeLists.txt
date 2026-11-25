cmake_minimum_required(VERSION 3.20)
project(ShadowVault VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(shadowvault_lib STATIC
    src/core/Entity.cpp
    src/items/Weapon.cpp
    src/items/Armor.cpp
    src/items/Consumable.cpp
    src/characters/Character.cpp
    src/characters/Player.cpp
    src/characters/Enemy.cpp
    src/world/Room.cpp
    src/world/DungeonMap.cpp
    src/game/Command.cpp
    src/game/CommandParser.cpp
    src/game/GameEngine.cpp
)

target_include_directories(shadowvault_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(shadowvault_lib PRIVATE -Wall -Wextra -Wpedantic)

add_executable(shadowvault src/main.cpp)
target_link_libraries(shadowvault PRIVATE shadowvault_lib)
target_compile_options(shadowvault PRIVATE -Wall -Wextra -Wpedantic)

enable_testing()
add_executable(shadowvault_tests tests/test_main.cpp)
target_link_libraries(shadowvault_tests PRIVATE shadowvault_lib)
target_compile_options(shadowvault_tests PRIVATE -Wall -Wextra -Wpedantic)
add_test(NAME ShadowVaultUnitTests COMMAND shadowvault_tests)
