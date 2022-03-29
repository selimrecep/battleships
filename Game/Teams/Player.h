#pragma once
#include <string>
#include "../Ships/Ship.h"
#include "PlayerColor.h"
#include "../Board/Grid.h"
#include "../Board/Point.h"

using ships_t = std::array<Ship, shipTypeCount>;

class Player {
private:
  void initializeShips();
  ships_t ships;
  std::string name;
public:
  PlayerColor color;
  int remainingShipCount{ shipTypeCount };

  static PlayerColor opposeTeamColorS(PlayerColor color);

  Player(PlayerColor color);
  void setName(std::string name);
  PlayerColor opposeTeamColor();
  std::string& getPlayerName();
  Ship* getShip(ShipType shipType);
};