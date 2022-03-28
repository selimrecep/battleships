#include "GameGrid.h"
#include "../Ships/ShipHitState.h"
#include "../Ships/Ship.h"

Point GameGrid::getDifferencePoint(Rotation rot) {
  Point point{ 0, 0 };

  switch (rot) {
  case Rotation::up:
    point.y = 1;
    break;
  case Rotation::down:
    point.y = -1;
    break;
  case Rotation::left:
    point.x = -1;
    break;
  case Rotation::right:
    point.x = 1;
    break;
  }
  return point;
}
bool GameGrid::fillCellsWithShip(const ShipCoordinate& shipCoordinate) {

  Point point{ shipCoordinate.prCompund.point };
  Point changerPoint{ GameGrid::getDifferencePoint(shipCoordinate.prCompund.rot) };
  if (!validCoordinate(shipCoordinate.prCompund, shipCoordinate.ship->type))  return false;
  for (int i = 0; i < shipCoordinate.ship->shipLength; ++i) {
    getCellByPoint(point).shipOccupying = shipCoordinate.ship;
    point.x += changerPoint.x;
    point.y += changerPoint.y;
  }
  return true;
}

// Owner class should update
ShipHitState GameGrid::tryHitPoint(Point point, Ship** ship) {
  if (!validPoint(point))
    return ShipHitState::INVALID_POINT;

  Ship* tempShip{ getCellByPoint(point).shipOccupying };

  if (tempShip == nullptr)
    return ShipHitState::MISS;

  *ship = tempShip;
  return ShipHitState::HIT;
}

bool GameGrid::validCoordinate(const PointRotCompound& compound, const ShipType shipType) {
  const Point& point{ compound.point };
  const Rotation& rot{ compound.rot };
  int length{ shipLengths[static_cast<int>(shipType)] };

  if (!validPoint(point))
    return false;

  const Point changerPoint{ GameGrid::getDifferencePoint(rot) };
  // Haven't fully learnt operator overloads yet
  const Point otherCorner{ point.x + changerPoint.x * length, point.y + changerPoint.y * length };
  if (!validPoint(otherCorner))
    return false;
  int i = 0;
  Point temp{ point };
  while (i < length) {
    temp.x += changerPoint.x;
    temp.y += changerPoint.y;
    if (getCellByPoint(temp).shipOccupying != nullptr)
      return false;
    i++;
  }
  return true;
}