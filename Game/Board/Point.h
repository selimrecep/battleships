#pragma once
#include <array>

struct Point {
  int x{};
  int y{};
};

enum class Rotation {
  up,
  down,
  left,
  right
};

struct PointRotCompound {
  Point point;
  Rotation rot{};
};



template <std::size_t T>
using coordinates_t = std::array<Point, T>;

template <std::size_t T>
using compoundCoords_t = std::array<PointRotCompound, T>;
