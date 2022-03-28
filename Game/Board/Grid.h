#pragma once
#include <array>
#include "Point.h"
#include "GameCell.h"
#include "HintCell.h"

// Hmm, maybe could add this into template instead
// but no point for this game
constexpr int gridEdgeLength = 10;


template<typename T>
using gridColumns_t = std::array<T, gridEdgeLength>;
template<typename T>
using gridMatrix_t = std::array<gridColumns_t<T>, gridEdgeLength>;

template<typename T>
class Grid {
private:
  gridMatrix_t<T> matrix;
public:
  Grid() : matrix{} {
  }

  T& getCellByPoint(Point point) {
    return matrix[point.y][point.x];
  }

  gridColumns_t<T> operator[](int row) {
    return matrix[row];
  }

  T& operator[](Point point) {
    return matrix[point.y][point.x];
  }

  bool validPoint(Point point) {
    return (point.x >= 0 && point.y >= 0
      && point.x < gridEdgeLength&& point.y < gridEdgeLength);
  }

};
