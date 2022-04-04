#pragma once
#include "../Board/HintGrid.h"
#include "../Board/Point.h"
#include "Player.h"

class ComputerAI {
private:
  bool initialized{false};
  Player* computerPlayer{nullptr};
  HintGrid* grid{nullptr};
  int hitShipCount{0};
  int rotTryCount{0};
  int lastHitShipLength{0};
  bool traversingOpposite{false};
  Rotation lastRotTry{Rotation::unkown};
  Rotation foundRot{Rotation::unkown};
  Point oppositeTraverseOrigin{invalidPoint};
  Point lastSuccessfulHit{invalidPoint};
  Point lastHitPoint{invalidPoint};

public:
  void initialize(Player& computer, HintGrid& grid);
  bool pointTriedBefore(Point point);
  Point playRound();
  void feedBackLastHit(bool hit);
};