#pragma once
#include "Board/GameGrid.h"
#include "Board/Grid.h"
#include "Board/HintGrid.h"
#include "Board/Point.h"
#include "Ships/Ship.h"
#include "Ships/ShipHitState.h"
#include "Teams/Player.h"
#include <array>
#include <string>

using players_t = std::array<Player, 2>;
using gameGrids_t = std::array<GameGrid, 2>;
using hintGrids_t = std::array<HintGrid, 2>;

enum class GameMode { v2, vComputer, computerVComputer };

class Game {
private:
  bool gameInitialized{false};
  bool gameEnded{false};
  GameMode gameMode{GameMode::v2};
  PlayerColor winnerColor{};
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
  void placeShips(const PlayerColor color,
                  const shipCoordinates_t<shipTypeCount>& compoundCoords);
  bool isGameEnded();
  PlayerColor getWinnerColor();

  ShipHitState attackPoint(const PlayerColor attackerColor, Point point,
                           bool& hasShipSunk);
  GameMode& getGameMode();
  void setGameMode(GameMode gameMode);
};
