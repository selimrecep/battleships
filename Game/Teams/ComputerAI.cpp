#include "ComputerAI.h"
#include "../../random.h"
#include "../Board/GameGrid.h"
#include "../Board/Point.h"
#include <iostream>

void ComputerAI::initialize(Player& computer, HintGrid& grid) {
  this->grid = &grid;
  this->computerPlayer = &computer;
}

Point ComputerAI::playRound() {
  Point chosenPoint{};
  if (foundRot != Rotation::unkown) {
    // Computers attacks accordingly to rotation known
    Point differer{GameGrid::getDifferencePoint(foundRot)};
    chosenPoint = lastSuccessfulHit + differer;

    if (!grid->validPoint(chosenPoint)) {
      chosenPoint = {getRandom(0, gridEdgeLength - 1),
                     getRandom(0, gridEdgeLength - 1)};
    }
  } else if (lastRotTry != Rotation::unkown) {
    // Computer doesn't know rotation yet, almost same code above but
    // wanted to split so I can debug easier
    Point differer{GameGrid::getDifferencePoint(lastRotTry)};

    bool alreadyHit{};
    chosenPoint = lastSuccessfulHit + differer;
    alreadyHit =
        (*grid)[chosenPoint.y][chosenPoint.x].state == HintCellState::hit;
    while (!grid->validPoint(chosenPoint) && alreadyHit) {
      chosenPoint = chosenPoint + differer;
      alreadyHit =
          (*grid)[chosenPoint.y][chosenPoint.x].state == HintCellState::hit;
    }
    if (alreadyHit || !grid->validPoint(chosenPoint)) {
      chosenPoint = {getRandom(0, gridEdgeLength - 1),
                     getRandom(0, gridEdgeLength - 1)};
    }
  } else {
    bool alreadyPicked{};
    do {
      chosenPoint = {getRandom(0, gridEdgeLength - 1),
                     getRandom(0, gridEdgeLength - 1)};
      alreadyPicked = pointTriedBefore(chosenPoint);
    } while (alreadyPicked);
  }
  if (!grid->validPoint(chosenPoint)) {
    std::cout << "w";
  }
  lastHitPoint = chosenPoint;
  return chosenPoint;
}
bool ComputerAI::pointTriedBefore(Point point) {
  return (*grid)[point.y][point.x].state != HintCellState::unkown;
}

void ComputerAI::feedBackLastHit(bool hit) {

  if (!hit) {
    // If it is a miss check if the AI was in ship points sequence
    if (foundRot != Rotation::unkown) {
      // If so, unset it
      // But check if we traversed opposite direction
      // In case of we might ve been hit ship in the middle
      if (!traversingOpposite) {
        traversingOpposite = true;
        lastSuccessfulHit = oppositeTraverseOrigin;

        lastRotTry = revertRotation(lastRotTry);
      } else {
        foundRot = Rotation::unkown;
        hitShipCount++;
        lastSuccessfulHit = invalidPoint;
        oppositeTraverseOrigin = invalidPoint;
        lastRotTry = Rotation::unkown;
        rotTryCount = 0;
        lastHitShipLength = 0;
        traversingOpposite = false;
      }
    } else if (lastRotTry != Rotation::unkown) {
      // Wrong rot. Try again with different rotation
      rotTryCount++;
      if (rotTryCount >= 4) {
        // well uh
        std::cout << "";
      }
      bool invalid{};
      Rotation rot{lastRotTry};

      int debug{0};
      do {
        rot = getNextRotation(rot);
        /*
        if (debug > 10)
          std::cout << "hmm ";*/
        // Actually it is a game grid we shouldn't been checked but it doesn't
        // matter since they have same size
        invalid =
            !grid->validPoint(lastHitPoint + GameGrid::getDifferencePoint(rot));
        debug++;
      } while (invalid);

      lastRotTry = rot;
    }
  } else {
    lastSuccessfulHit = lastHitPoint;
    // If it is a hit

    if (foundRot != Rotation::unkown) {
      // Check if it is a valid move.
      Point chosenPoint{};
      Point differer{GameGrid::getDifferencePoint(foundRot)};
      chosenPoint = lastSuccessfulHit + differer;

      if (!grid->validPoint(chosenPoint)) {

        foundRot = Rotation::unkown;
        hitShipCount++;
        lastSuccessfulHit = invalidPoint;
        oppositeTraverseOrigin = invalidPoint;
        lastRotTry = Rotation::unkown;
        rotTryCount = 0;
        lastHitShipLength = 0;
        traversingOpposite = false;
      }
    }

    if (lastRotTry == Rotation::unkown) {
      // Initialize first rotation try
      bool invalid{};
      Rotation rot{Rotation::unkown};
      do {
        rot = static_cast<Rotation>(getRandom(0, 3));

        // Actually it is a game grid we shouldn't been checked but it doesn't
        // matter since they have same size
        invalid =
            !grid->validPoint(lastHitPoint + GameGrid::getDifferencePoint(rot));
      } while (invalid);
      lastRotTry = rot;
    } else {
      // Found the correct rot.
      oppositeTraverseOrigin = lastHitPoint;
      foundRot = lastRotTry;
    }
  }
}