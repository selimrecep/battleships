#pragma once
#include "../Game/Game.h"
#include "../Game/Teams/ComputerAI.h"
#include "ConsoleBuffer.h"
#include <array>
#include <string>

class GuiManager {
private:
  Game& game;
  ComputerAI ai1;
  ComputerAI ai2;
  ConsoleBuffer cBuffer;
  void getFullLineText(std::string& line);

public:
  GuiManager(Game& game);

  void welcomeText();
  void askForNames();
  void askForGameMode();
  void beginGame();

  void setGameMode(GameMode gameMode);

  void printGameGrid(const PlayerColor color);
  void printHintGrid(const PlayerColor color);
  void oneRound(const PlayerColor color);

  void registerShips();
  void registerShip(const PlayerColor color);
  void registerShipRandom(const PlayerColor color);
  Point getCoordinateFromUser();

  void printPoint(Point point);
};