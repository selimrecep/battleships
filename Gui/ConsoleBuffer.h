#pragma once
#include <string>

constexpr int consoleWidth = 150;
constexpr int consoleHeight = 150;

class ConsoleBuffer {
private:
  std::string consoleBuffer;
  int posX{ 0 };
  int posY{ 0 };
public:
  ConsoleBuffer();
  void cleanConsoleBuffer();
  void printBuffer();
  char& operator()(int column, int row);
  void clearScreen();
  void setCurrentPos(int x, int y);
};