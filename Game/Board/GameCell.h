#pragma once
#include "../Ships/Ship.h"

struct GameCell {
  Ship* shipOccupying{ nullptr };
  bool shotDown{ false };
};