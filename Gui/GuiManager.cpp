#include "GuiManager.h"
#include "../Game/Board/GameCell.h"
#include "../Game/Board/Grid.h"
#include "../Game/Board/HintCell.h"
#include "../Game/Board/HintGrid.h"
#include "../Game/Game.h"
#include "../random.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

GuiManager::GuiManager(Game& game) : game(game) {
  cBuffer.cleanConsoleBuffer();
}

void GuiManager::welcomeText() {
  std::cout << "---------------------------Welcome to "
               "battleships!-----------------\n";
  std::cout << "Please don't forget to increase terminal width! The app can't "
               "do it manually, yet :)\n";
}
void GuiManager::askForGameMode() {
  std::cout << "Which game mode do you want to play?\n";
  std::cout << "1: User vs User\n";
  std::cout << "2: User vs Computer\n";
  std::cout << "3: Computer vs Computer\n";

  int gameModeId{};
  std::cin >> gameModeId;

  if (gameModeId < 1 || gameModeId > 3) {
    std::cout << "Invalid game mode.\n";
    askForGameMode();
    return;
  }

  gameModeId--;

  setGameMode(static_cast<GameMode>(gameModeId));
}

void GuiManager::askForNames() {
  using std::chrono::seconds;
  using std::this_thread::sleep_for;
  std::string redPlayerName{"computer1"}, bluePlayerName{"computer2"};
  if (game.getGameMode() != GameMode::computerVComputer) {
    std::cout << "What is the name of red player?: ";
    getFullLineText(redPlayerName);
  }
  if (game.getGameMode() == GameMode::v2) {
    std::cout << "And now, what is the name of blue player?: ";
    getFullLineText(bluePlayerName);
  }

  // TODO: Check if I implemented std::move correctly... EDIT: No I didn't
  game.createPlayers(redPlayerName, bluePlayerName);

  std::cout << "Players are registered! Game begins!\n";
  sleep_for(seconds(1));
  beginGame();
}
void GuiManager::setGameMode(GameMode gameMode) { game.setGameMode(gameMode); }
void GuiManager::beginGame() {
  if (game.getGameMode() == GameMode::vComputer) {
    ai1.initialize(game.getPlayer(PlayerColor::blue),
                   game.getHintGrid(PlayerColor::blue));
  } else if (game.getGameMode() == GameMode::computerVComputer) {
    ai1.initialize(game.getPlayer(PlayerColor::red),
                   game.getHintGrid(PlayerColor::red));
    ai2.initialize(game.getPlayer(PlayerColor::blue),
                   game.getHintGrid(PlayerColor::blue));
  }

  cBuffer.clearScreen();
  registerShips();
  PlayerColor color{PlayerColor::red};

  while (!game.isGameEnded()) {
    oneRound(color);
    color = Player::opposeTeamColorS(color);
  }
  Player& winner = game.getPlayer(game.getWinnerColor());
  std::cout << "The winner is "
            << ((game.getWinnerColor() == PlayerColor::red) ? "red" : "blue")
            << "(a.k.a.) " << winner.getPlayerName();
}
void GuiManager::registerShips() {
  if (game.getGameMode() != GameMode::computerVComputer)
    registerShip(PlayerColor::red);
  else
    registerShipRandom(PlayerColor::red);
  if (game.getGameMode() == GameMode::v2)
    registerShip(PlayerColor::blue);
  else
    registerShipRandom(PlayerColor::blue);
}

void GuiManager::registerShipRandom(const PlayerColor color) {
  Player& player = game.getPlayer(color);
  GameGrid& gGrid = game.getGameGrid(color);

  int i{0};
  bool invalid{};
  while (i < shipTypeCount) {
    ShipCoordinate shipCoordinate;
    ShipType shipType{static_cast<ShipType>(i)};
    Ship* ship = player.getShip(shipType);
    shipCoordinate.ship = ship;
    shipCoordinate.prCompund.rot = static_cast<Rotation>(getRandom(0, 3));
    shipCoordinate.prCompund.point.y = getRandom(0, gridEdgeLength - 1);
    shipCoordinate.prCompund.point.x = getRandom(0, gridEdgeLength - 1);
    invalid = !gGrid.fillCellsWithShip(shipCoordinate);
    // Keep trying until ship adding operation succeds, definitely not the best
    // approach...
    if (!invalid)
      i++;
  }
}

void GuiManager::registerShip(const PlayerColor color) {
  Player& player = game.getPlayer(color);

  std::cout << player.getPlayerName() << " will enter ships.\n\n";
  std::string input{};
  GameGrid& gGrid = game.getGameGrid(color);

  for (int i = 0; i < shipTypeCount; i++) {
    printGameGrid(color);
    bool invalid{false};
    do {
      std::cout << shipNames[i] << " the ship(length = " << shipLengths[i]
                << "), coords(e.g. A0): ";
      std::cin >> input;
      char letterCh{input[0]}, numberCh{input[1]};
      if (letterCh < 'A' || letterCh > 'J' || numberCh < '0' ||
          numberCh > '9') {
        std::cout << "Invalid input.\n";
        invalid = true;
      } else {
        invalid = false;
        std::cout << "Now enter rotation(1-4): ";
        int rot{};
        std::cin >> rot;
        if (rot < 1 || rot > 4) {
          std::cout << "Invalid rot.\n";
          invalid = true;
        } else {
          rot--;
          invalid = false;
          int row{input[0] - 'A'};
          int column{input[1] - '0'};
          ShipCoordinate shipCoordinate;
          ShipType shipType{static_cast<ShipType>(i)};
          Ship* ship = player.getShip(shipType);
          shipCoordinate.ship = ship;
          shipCoordinate.prCompund.rot = static_cast<Rotation>(rot);
          shipCoordinate.prCompund.point.y = row;
          shipCoordinate.prCompund.point.x = column;
          invalid = !gGrid.fillCellsWithShip(shipCoordinate);
          if (invalid)
            std::cout << "Invalid position.\n";
        }
      }
    } while (invalid);
  }
  printGameGrid(color);
  std::cout << "\n-------------\n";
}

Point GuiManager::getCoordinateFromUser() {
  int row{};
  int column{};
  bool invalid{false};
  std::string input{};
  do {
    std::cout << "Enter coordinate: ";
    std::cin >> input;
    char letterCh{input[0]}, numberCh{input[1]};
    if (letterCh < 'A' || letterCh > 'J' || numberCh < '0' || numberCh > '9') {
      std::cout << "Invalid input.\n";
      invalid = true;
    } else {
      invalid = false;
      row = input[0] - 'A';
      column = input[1] - '0';
    }
  } while (invalid);

  Point point{column, row};
  return point;
}

void GuiManager::getFullLineText(std::string& line) {
  // Ignore any leading whitespace left
  // I still don't know enough about std::ws's purpose here
  // and mix it up with something else but, it works :P
  std::getline(std::cin >> std::ws, line);
}
void GuiManager::printPoint(Point point) {
  std::cout << (static_cast<char>(point.y + 'A')) << (point.x);
}
void GuiManager::oneRound(const PlayerColor color) {
  using std::chrono::seconds;
  using std::this_thread::sleep_for;
  Player& player = game.getPlayer(color);

  std::cout << "It is " << player.getPlayerName()
            << "'s turn. Other player should avert their eyes.\n";
  sleep_for(seconds(0));

  std::cout << "-------- YOUR SHIPS ------\n";
  printGameGrid(color);

  std::cout << "----- YOUR HINT TABLE ----\n";
  printHintGrid(color);

  if ((color == PlayerColor::blue && game.getGameMode() != GameMode::v2) ||
      game.getGameMode() == GameMode::computerVComputer) {
    std::cout << "It is a COMPUTER TURN!\n";

    ComputerAI& ai = (game.getGameMode() == GameMode::computerVComputer)
                         ? ((color == PlayerColor::red) ? ai1 : ai2)
                         : (ai1);

    Point point{ai.playRound()};
    std::cout << "Computer has chosen ";
    printPoint(point);
    std::cout << "\n";

    bool hasShipSunk{};
    ShipHitState state{game.attackPoint(color, point, hasShipSunk)};
    if (state == ShipHitState::HIT) {
      std::cout << "It is a HIT!\n";
      ai.feedBackLastHit(true);
    } else {
      std::cout << "It is a miss.\n";
      ai.feedBackLastHit(false);
    }

    return;
  }

  Point point{getCoordinateFromUser()};
  bool hasShipSunk{};
  ShipHitState state{game.attackPoint(player.color, point, hasShipSunk)};

  if (state == ShipHitState::HIT) {
    std::cout << "It is a hit!\n";
    if (hasShipSunk) {
      std::cout << "Other player: \"You have sunken my ship!\"\n";
    }
  } else if (state == ShipHitState::MISS) {
    std::cout << "It is a miss.\n";
  } else if (state == ShipHitState::INVALID_POINT) {
    std::cout << "Invalid point. Your turn is forfeited.\n";
  }
}

void GuiManager::printHintGrid(const PlayerColor color) {

  HintGrid& grid = game.getHintGrid(color);

  cBuffer.setCurrentPos(0, 0);

  HintCell cell;
  char ch{};
  char letter{'A'};
  std::cout << "   ";
  for (int i = 0; i < gridEdgeLength; i++)
    std::cout << letter++ << " ";

  std::cout << "\n  .";
  for (int i = 0; i < gridEdgeLength - 1; i++)
    std::cout << "--";
  std::cout << ".\n";
  for (int row = 0; row < gridEdgeLength; ++row) {

    std::cout << (row) << " ";
    for (int col = 0; col < gridEdgeLength; ++col) {
      cell = grid[col][row];

      if (cell.state == HintCellState::unkown)
        ch = ' ';
      else if (cell.state == HintCellState::hit)
        ch = 'x';
      else
        ch = '-';
      std::cout << '|';
      std::cout << ch;
    }

    std::cout << "|\n";
  }

  std::cout << "  .";
  for (int i = 0; i < gridEdgeLength - 1; i++)
    std::cout << "--";
  std::cout << ".\n";
}

void GuiManager::printGameGrid(const PlayerColor color) {
  GameGrid& grid = game.getGameGrid(color);

  cBuffer.setCurrentPos(0, 0);

  GameCell cell;
  char ch{};
  char letter{'A'};
  std::cout << "   ";
  for (int i = 0; i < gridEdgeLength; i++)
    std::cout << letter++ << " ";

  std::cout << "\n  .";
  for (int i = 0; i < gridEdgeLength - 1; i++)
    std::cout << "--";
  std::cout << ".\n";

  for (int row = 0; row < gridEdgeLength; ++row) {
    std::cout << (row) << " ";
    for (int col = 0; col < gridEdgeLength; ++col) {
      cell = grid[col][row];

      if (cell.shipOccupying != nullptr) {
        if (cell.shotDown)
          ch = 'x';
        else
          ch = '.';
      } else {
        ch = ' ';
      }
      std::cout << '|';
      std::cout << ch;
    }

    std::cout << "|\n";
  }

  std::cout << "  .";
  for (int i = 0; i < gridEdgeLength - 1; i++)
    std::cout << "--";
  std::cout << ".\n";
}