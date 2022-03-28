#pragma once
#include "Grid.h"
#include "GameCell.h"
#include "../Ships/ShipHitState.h"

class GameGrid : public Grid<GameCell> {
public:
  static Point getDifferencePoint(Rotation rot);

  bool fillCellsWithShip(const ShipCoordinate& shipCoordinate);
  bool validCoordinate(const PointRotCompound& compound, const ShipType shipType);
  /* shouldn't be called directly */
  ShipHitState tryHitPoint(Point point, Ship** ship);
};