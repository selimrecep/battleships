#pragma once
#include <array>

constexpr int gridEdgeLength = 10;

class Grid {
private:
  std::array<std::array<int, gridEdgeLength>, gridEdgeLength> matrix;
public:
  Grid();
};
