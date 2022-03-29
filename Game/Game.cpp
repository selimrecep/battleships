#include <cassert>
#include <string>
#include "Board/GameGrid.h"
#include "Game.h"
#include "Teams/PlayerColor.h"
#include "Ships/Ship.h"


Game::Game() : players{ PlayerColor::red, PlayerColor::blue } {
}


Player& Game::getPlayer(const PlayerColor color) {
  return players[static_cast<int>(color)];
}
GameGrid& Game::getGameGrid(const PlayerColor color) {
  return gameGrids[static_cast<int>(color)];
}
HintGrid& Game::getHintGrid(const PlayerColor color) {
  return hintGrids[static_cast<int>(color)];
}
void Game::createPlayers(std::string redName, std::string blueName) {
  assert(!gameInitialized && "Game is already initialized.");
  Player& redPlayer{ getPlayer(PlayerColor::red) };
  Player& bluePlayer{ getPlayer(PlayerColor::blue) };

  redPlayer.setName(std::move(redName));
  bluePlayer.setName(std::move(blueName));

  gameInitialized = true;
}

void Game::placeShips(const PlayerColor color, const shipCoordinates_t<shipTypeCount>& compoundCoords) {
  GameGrid& grid{ getGameGrid(color) };

  for (int i = 0; i < shipTypeCount; i++) {
    grid.fillCellsWithShip(compoundCoords[i]);
  }
}


ShipHitState Game::attackPoint(const PlayerColor attackerColor, Point point, bool& isShipSunk) {
  Player& attacker{ getPlayer(attackerColor) };
  const PlayerColor defenderColor{ attacker.opposeTeamColor() };
  Player& defender{ getPlayer(defenderColor) };

  GameGrid& defGameGrid{ getGameGrid(defenderColor) };
  HintGrid& atkHintGrid{ getHintGrid(attackerColor) };
  Ship* ship;
  ShipHitState result{ defGameGrid.tryHitPoint(point, &ship) };

  if (result == ShipHitState::HIT) {
    // It is a hit
    ship->hitTimes++;
    defGameGrid[point].shotDown = true;

    atkHintGrid[point].state = HintCellState::hit;
    if (ship->hitTimes == ship->shipLength) {
      isShipSunk = true;
      ship->hasSunk = true;
      defender.remainingShipCount--;


      if (defender.remainingShipCount == 0) {
        endGame(attacker);
      }
    }
  }
  else if (result == ShipHitState::MISS) {
    // It is a miss
    atkHintGrid[point].state = HintCellState::miss;
  }

  return result;
}

void Game::endGame(const Player& winner) {
  gameEnded = true;
  winnerColor = winner.color;
}

bool Game::isGameEnded() {
  return gameEnded;
}

PlayerColor Game::getWinnerColor() {
  return winnerColor;
}