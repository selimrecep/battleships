#pragma once
#include <array>
#include "../Board/Point.h"

enum class ShipType {
  carrier,
  battleship,
  cruiser,
  submarine,
  destroyer,

  maxShipType
};


constexpr int shipTypeCount = static_cast<int>(ShipType::maxShipType);

constexpr std::array<int, shipTypeCount> shipLengths{ 5, 4, 3, 3, 2 };
const std::array<std::string, shipTypeCount> shipNames{
  "Carrier", "Battleship","Crusier", "Submarine", "Destroyer"
};

struct Ship {
  int shipLength{};
  int hitTimes{ 0 };
  int hasSunk{ false };
  ShipType type{};
};

struct ShipCoordinate {
  Ship* ship{ nullptr };
  PointRotCompound prCompund;
};

template <std::size_t T>
using shipCoordinates_t = std::array<ShipCoordinate, T>;