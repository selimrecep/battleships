#include <iostream>
#include "Game/Game.h"
#include "Game/Board/GameCell.h"
#include "Game/Board/Point.h"
#include "Gui/GuiManager.h"

int main() {
  Game game{};
  GuiManager gManager{ game };

  gManager.welcomeText();
  gManager.askForNames();

  int a;
  std::cout << "Press a key and hit enter to exit (no white space please :) )\n";
  std::cin >> a;
}