#include "Point.h"

Rotation getNextRotation(Rotation rot) {
  switch (rot) {
  case Rotation::up:
    return Rotation::right;
  case Rotation::right:
    return Rotation::down;
  case Rotation::down:
    return Rotation::left;
  case Rotation::left:
    return Rotation::up;
  default:
    return Rotation::unkown;
  }
}

Rotation revertRotation(Rotation rot) {
  switch (rot) {
  case Rotation::up:
    return Rotation::down;
  case Rotation::right:
    return Rotation::left;
  case Rotation::down:
    return Rotation::up;
  case Rotation::left:
    return Rotation::right;
  default:
    return Rotation::unkown;
  }
}