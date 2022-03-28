#pragma once
#include <array>
#include <string>
#include "../Game/Game.h"
#include "ConsoleBuffer.h"


class GuiManager {
private:
  Game& game;
  ConsoleBuffer cBuffer;
  void getFullLineText(std::string& line);
public:
  GuiManager(Game& game);

  void welcomeText();
  void askForNames();
  void beginGame();

  void printGameGrid(const PlayerColor color);
  void printHintGrid(const PlayerColor color);
  void printAllGrids(const PlayerColor color);

  void registerShips();
  void registerShip(const PlayerColor color);
};