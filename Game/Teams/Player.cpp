#include <string>
#include "PlayerColor.h"
#include "Player.h"
#include "../Ships/Ship.h"

Player::Player(PlayerColor color) : color{ color } {
  initializeShips();
}

/**
 * @brief Initialises ships' length
 */
void Player::initializeShips() {
  for (int i = 0; i < static_cast<int>(ShipType::maxShipType); ++i) {
    ships[i].shipLength = shipLengths[i];
    ships[i].type = static_cast<ShipType>(i);
  }
}

// Move semantics are soooo confusing :'(
// Will revisit later after I learn the proper concepts first.
// Including value types, constructor types (including moving ones), etc.
void Player::setName(std::string name) {
  this->name = std::move(name);
}

std::string& Player::getPlayerName() {
  return name;
}

PlayerColor Player::opposeTeamColor() {
  // Ngl a bit dirty :)
  return static_cast<PlayerColor>(!static_cast<int>(color));
}

PlayerColor Player::opposeTeamColorS(PlayerColor color) {
  // Ngl a bit dirty :)
  return static_cast<PlayerColor>(!static_cast<int>(color));
}

Ship* Player::getShip(ShipType shipType) {
  return &ships[static_cast<int>(shipType)];
}