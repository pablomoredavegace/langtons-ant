
#include "tape.h"
#include <iostream>
#include <stdexcept>


Tape::Tape(size_t sizeX, size_t sizeY) : sizeX(sizeX), sizeY(sizeY), cells(sizeX * sizeY, 0) {}

bool Tape::Limits(int x, int y) const {
  if( (x >= 0) && (y >=0) && (static_cast<std::size_t>(x) < sizeX) && (static_cast<std::size_t>(y) < sizeY)) {
    return true;
    }
  else return false;
}

std::size_t Tape::PositionIndex(int x, int y) const {
  return static_cast<std::size_t>(y) * sizeX + static_cast<std::size_t>(x);
}

bool Tape::IsBlack(int x, int y) const {
  if(!Limits(x, y)) {
    throw std::out_of_range("La celda está fuera de rango");
  }
  return cells[PositionIndex(x, y) != 0];
}

void Tape::SetBlack(int x, int y, bool black) {
  if(!Limits(x, y)) {
    throw std::out_of_range("La celda está fuera de rango");
  }
  cells[PositionIndex(x, y)] = black ? 1 : 0;
}

void Tape::CambioColor(int x, int y) {
  if(!Limits(x, y)) {
    throw std::out_of_range("La celda está fuera de rango");
  }
  cells[PositionIndex(x, y)] = cells[PositionIndex(x, y)] ? 0 : 1;
}

std::vector<std::pair<int, int>> Tape::BlackCells() const {
  std::vector<std::pair<int, int>> out;
  for(std::size_t y = 0; y < sizeY; y++) {
    for(std::size_t x = 0; x < sizeX; x++){
      if(cells[y * sizeX + x]) {
        out.push_back({static_cast<int>(x), static_cast<int>(y)});
      }
    }
  }
  return out;
}

std::ostream& operator<<(std::ostream& os, const Tape& tape) {
  for(std::size_t y = 0; y < tape.sizeY; y++) {
    for(std::size_t x = 0; x < tape.sizeX; x++) {
      os << (tape.cells[y * tape.sizeX + x] ? 'X' : ' ');
    }
    os << '\n'; 
  }
  return os;
}