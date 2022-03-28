#include <iostream>
#include <string>
#include <thread>
#include <cstdlib>
#include "GuiManager.h"
#include "../Game/Game.h"
#include "../Game/Board/Grid.h"
#include "../Game/Board/HintGrid.h"
#include "../Game/Board/HintCell.h"
#include "../Game/Board/GameCell.h"

GuiManager::GuiManager(Game& game) : game(game) {
  cBuffer.cleanConsoleBuffer();
}

void GuiManager::welcomeText() {
  std::cout << "---------------------------Welcome to battleships!-----------------\n";
  std::cout << "Please don't forget to increase terminal width! The app can't do it manually, yet :)\n";
}

void GuiManager::askForNames() {
  using std::this_thread::sleep_for;
  using std::chrono::seconds;

  std::string redPlayerName, bluePlayerName;
  std::cout << "What is the name of red player?: ";
  getFullLineText(redPlayerName);
  std::cout << "And now, what is the name of blue player?: ";
  getFullLineText(bluePlayerName);

  //TODO: Check if I implemented std::move correctly...
  game.createPlayers(redPlayerName, bluePlayerName);

  std::cout << "Players are registered! Game begins!\n";
  sleep_for(seconds(1));
  beginGame();
}

void GuiManager::beginGame() {
  cBuffer.clearScreen();
  registerShips();

  printAllGrids(PlayerColor::red);
}
void GuiManager::registerShips() {
  registerShip(PlayerColor::red);
  registerShip(PlayerColor::blue);
}

void GuiManager::registerShip(const PlayerColor color) {
  Player& player = game.getPlayer(color);

  std::cout << player.getPlayerName() << " will enter ships.\n\n";
  std::string input{};
  GameGrid& gGrid = game.getGameGrid(color);

  for (int i = 0; i < shipTypeCount; i++) {
    printGameGrid(color);
    bool invalid{ false };
    do {
      std::cout << shipNames[i] << " the ship(length = " << shipLengths[i] << "), coords(e.g. A0): ";
      std::cin >> input;
      char letterCh{ input[0] }, numberCh{ input[1] };
      if (letterCh < 'A' || letterCh > 'J' || numberCh < '0' || numberCh > '9') {
        std::cout << "Invalid input.\n";
        invalid = true;
      }
      else {
        invalid = false;
        std::cout << "Now enter rotation(1-4): ";
        int rot{};
        std::cin >> rot;
        if (rot < 1 || rot > 4) {
          std::cout << "Invalid rot.\n";
          invalid = true;
        }
        else {
          rot--;
          invalid = false;
          int row{ input[0] - 'A' };
          int column{ input[1] - '0' };
          ShipCoordinate shipCoordinate;
          ShipType shipType{ static_cast<ShipType>(i) };
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

void GuiManager::getFullLineText(std::string& line) {
  // Ignore any leading whitespace left
  // I still don't know enough about std::ws's purpose here
  // and mix it up with something else but, it works :P
  std::getline(std::cin >> std::ws, line);
}

void GuiManager::printAllGrids(const PlayerColor color) {
  using std::this_thread::sleep_for;
  using std::chrono::seconds;
  Player& player = game.getPlayer(color);

  std::cout << "It is " << player.getPlayerName() << "'s turn. Other player should avert their eyes.\n";
  sleep_for(seconds(0));

  std::cout << "-------- YOUR SHIPS ------\n";
  printGameGrid(color);

  std::cout << "----- YOUR HINT TABLE ----\n";
  printHintGrid(color);
}

void GuiManager::printHintGrid(const PlayerColor color) {

  HintGrid& grid = game.getHintGrid(color);


  cBuffer.setCurrentPos(0, 0);

  HintCell cell;
  char ch{};
  char letter{ 'A' };
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
  char letter{ 'A' };
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
      }
      else {
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
/*
void GuiManager::printGameGrid(const PlayerColor color) {
  Player& player = game.getPlayer(color);
  GameGrid& grid = game.getGameGrid(color);

  cBuffer.setCurrentPos(0, 0);

  GameCell cell;
  char ch;
  for (int row = 0; row < gridEdgeLength; ++row) {

    for (int col = 0; col < gridEdgeLength; ++col) {
      cell = grid[row][col];

      if (cell.shipOccupying != nullptr) {
        if (cell.shotDown)
          ch = 'x';
        else
          ch = '.';
      }
      else {
        ch = ' ';
      }
      cBuffer(2 * col, row) = '|';
      cBuffer(2 * col + 1, row) = ch;
    }

    cBuffer((gridEdgeLength - 1) * 2, row) = '\n';
  }

  cBuffer.printBuffer();
}*/