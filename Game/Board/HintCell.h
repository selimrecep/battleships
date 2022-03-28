#pragma once

enum class HintCellState {
  unkown,
  hit,
  miss
};
struct HintCell {
  HintCellState state;
};