#pragma once
#include <array>

struct Point {
  int x{};
  int y{};
  Point operator+(const Point& point) {
    return Point{x + point.x, y + point.y};
  }
};

constexpr Point invalidPoint{-1, -1};

enum class Rotation { up, down, left, right, unkown };

struct PointRotCompound {
  Point point;
  Rotation rot{};
};

template <std::size_t T> using coordinates_t = std::array<Point, T>;

template <std::size_t T>
using compoundCoords_t = std::array<PointRotCompound, T>;

Rotation getNextRotation(Rotation rot);
Rotation revertRotation(Rotation rot);