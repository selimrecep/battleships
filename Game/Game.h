#pragma once
#include <string>
#include <array>
#include "Teams/Player.h"
#include "Ships/Ship.h"
#include "Ships/ShipHitState.h"
#include "Board/Grid.h"
#include "Board/Point.h"
#include "Board/GameGrid.h"
#include "Board/HintGrid.h"

using players_t = std::array<Player, 2>;
using gameGrids_t = std::array<GameGrid, 2>;
using hintGrids_t = std::array<HintGrid, 2>;

class Game {
private:
  bool gameInitialized{};
  bool gameEnded{ false };
  PlayerColor winnerColor{  };
  players_t players;
  gameGrids_t gameGrids;
  hintGrids_t hintGrids;

  void endGame(const Player& winner);
public:
  Game();
  GameGrid& getGameGrid(const PlayerColor color);
  HintGrid& getHintGrid(const PlayerColor color);
  Player& getPlayer(const PlayerColor color);
  void createPlayers(std::string redName, std::string blueName);
  void placeShips(const PlayerColor color, const shipCoordinates_t<shipTypeCount>& compoundCoords);
  bool isGameEnded();
  PlayerColor getWinnerColor();

  ShipHitState attackPoint(const PlayerColor attackerColor, Point point, bool& hasShipSunk);
};
