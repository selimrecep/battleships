#include <iostream>
#include "ConsoleBuffer.h"
#include <cassert>

ConsoleBuffer::ConsoleBuffer() : consoleBuffer(consoleWidth* consoleHeight, '.') {}
void ConsoleBuffer::printBuffer() {
  std::cout << consoleBuffer;
}

void ConsoleBuffer::cleanConsoleBuffer() {
  for (int i = 0; i < consoleHeight; ++i)
    std::cout << std::string(consoleWidth, ' ');
}


char& ConsoleBuffer::operator()(int column, int row) {
  assert((posY + row) * consoleWidth + (posX + column) < (consoleWidth * consoleHeight));
  return consoleBuffer[(posY + row) * consoleWidth + (posX + column)];
}

void ConsoleBuffer::setCurrentPos(int x, int y) {
  posX = x;
  posY = y;
}

void ConsoleBuffer::clearScreen() {
  cleanConsoleBuffer();
}
